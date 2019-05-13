#include <stdlib.h>

#include <fstream>
#include <iostream>
#include <string>

#include "gumbo.h"

std::ofstream ofs("hrefList.txt", std::ios::out | std::ofstream::binary);
static void search_for_links(GumboNode *node)
{
  if (node->type != GUMBO_NODE_ELEMENT)
  {
    return;
  }
  GumboAttribute *href;
  if (node->v.element.tag == GUMBO_TAG_A &&
      (href = gumbo_get_attribute(&node->v.element.attributes, "href")))
  {
    //std::cout << href->value << std::endl;
    ofs<< href->value <<std::endl;
  }

  GumboVector *children = &node->v.element.children;
  for (unsigned int i = 0; i < children->length; ++i)
  {
    search_for_links(static_cast<GumboNode *>(children->data[i]));
  }
}

extern "C" void getHref(char* file_name)
{
  //const char *filename = "file_name";
  const char *filename = "output1.txt";
  std::ifstream in(filename, std::ios::in | std::ios::binary);
  if (!in)
  {
    std::cout << "File " << filename << " not found!\n";
    exit(EXIT_FAILURE);
  }

  std::string contents;
  in.seekg(0, std::ios::end);
  contents.resize(in.tellg());
  in.seekg(0, std::ios::beg);
  in.read(&contents[0], contents.size());
  in.close();

  GumboOutput *output = gumbo_parse(contents.c_str());
  search_for_links(output->root);
  gumbo_destroy_output(&kGumboDefaultOptions, output);

  ofs.close();
}
