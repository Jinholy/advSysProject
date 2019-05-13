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


###  * gumbo-parser :  
#### clone gumbo-parser
> git clone https://github.com/google/gumbo-parser.git  
#### install tools  
> sudo apt-get install libtool  
> sudo apt-get install automake  
#### type on command line at gumbo-parser directory
> cd gumbo-parser  
> ./autogen.sh  
> ./configure  
> make  
> sudo make install  
> sudo apt-get install libgomp1  

### How to compile  
> g++ -o main main.c -lcurl -lgumbo  

### How to run  
> ./main .ext url  
> ex) ./main .zip https://iscxdownloads.cs.unb.ca/iscxdownloads/CICAndMal2017/CSVs/