# advSysProject

Our goal is to make an ~~awesome~~ Multi-threaded Web-crawler  
JINHO LEE 2018227155  
DONXING CAO 2018227235  
SEONHYEONG KIM 2018227098  

***KNU ADVANCED SYSTEM PROJECT TERM PROJECT***

    
## Dependencies
###  * libcurl : 
### move to the curl-7.64.1 directory and  
> ./configure --prefix=/usr/share/man --with-ssl  
> make && make install  
#### If there are problem with permission please do with root Or  

> sudo apt-get install curl  
#### you can check with  
> curl -V  
> curl naver.com  
#### at command line if command prints something. installed well.  


###  * TidyLib :  
#### clone tidy 
> git clone https://github.com/htacg/tidy-html5.git  
#### build tidy library  
> cd build/cmake  
> cmake ../.. -DCMAKE_BUILD_TYPE=Release  
> make  
> sudo make install  
#### if you have problem with build tidy, read this page
> https://github.com/htacg/tidy-html5/blob/next/README/BUILD.md
## References  
* https://github.com/htacg/tidy-html5
* 