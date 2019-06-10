#include <stdlib.h>

#include <fstream>
#include <iostream>
#include <string>

#include "gumbo.h"

std::ofstream ofs("hrefList.txt", std::ios::out | std::ofstream::binary);
//search_for_links가 재귀적으로 돌아가는 함수라 함수 안에다가 정의하니 계속 다시써서 hrefList.txt에 남는게 없어짐．
//그렇다고 app옵션으로 하려하니 프로그램 실행할때 마다 뒤에 append되서 밖에다가 선언함． 

static void search_for_links(GumboNode *node)
{
  if (node->type != GUMBO_NODE_ELEMENT)
  {
    return;
  }
  GumboAttribute *href;
  if (node->v.element.tag == GUMBO_TAG_A &&
      (href = gumbo_get_attribute(&node->v.element.attributes, "href")))    
      //href태그만 뽑아서 GumboAttribute에 넣어놓는다． 나중에 image에 대한 Tag도 추출할 수 있게 개선해야할듯
  {
    //std::cout << href->value << std::endl;
    ofs<< href->value <<std::endl;          //href태그만 뽑아놓은 거를 ofstream을 통해 hrefList.txt에 씀
  }

  //재귀적으로 안으로 들어가서 반복하는 함수인듯한데 제대로 잘 안된다． root에 대한 child만 뽑아내는듯함． 수정해야할 사항．
  GumboVector *children = &node->v.element.children;
  for (unsigned int i = 0; i < children->length; ++i)
  {
    search_for_links(static_cast<GumboNode *>(children->data[i]));      
  }
}

extern "C" void getHref(const char* file_name)
{
  //지금 gumboparser는 로컬에 있는 파일을 읽어들여서 거기서 href태그만 뽑아다가 출력하는 방식．
  //const char *filename = "file_name";
  const char *filename = "output1.txt";   //파라메터로 받으니까 이상하게 파일 못찾겠다고 해서 일단 하드코딩했다．
  std::ifstream in(filename, std::ios::in | std::ios::binary);
  if (!in)
  {
    std::cout << "File " << filename << " not found!\n";
    exit(EXIT_FAILURE);
  }

  //ifstream에 왜 이런거해논지모르겠음． 그냥 넘어가자．
  //파라매터로 넘겨받은 file_name(지금은 output1.txt)을 읽어서 contents에 저장함．
  std::string contents;
  in.seekg(0, std::ios::end);
  contents.resize(in.tellg());
  in.seekg(0, std::ios::beg);
  in.read(&contents[0], contents.size());
  in.close();

  GumboOutput *output = gumbo_parse(contents.c_str()); //contests를 읽어서 파싱함． tree형식으로 만드는 과정인듯．
  search_for_links(output->root);                       //결과물로 루트부터 search_for_links를 통해 href태그만 뽑아냄．
  gumbo_destroy_output(&kGumboDefaultOptions, output);

  ofs.close();    //ofstream을 닫아준다．
}
