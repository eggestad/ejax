
# 8.4-lkgr
v8obj=$(HOME)/v8/v8/out.gn/x64.release.sample/obj
v8inc=$(HOME)/v8/v8/

#CXX=clang++
CC=gcc

all: shell testEjax shell-old ejax-cmd

CFLAGS=-I. -I./include -I./ejax/include

v8obj=$(HOME)/v8/11.2/v8/out.gn/x64.release.sample/obj
v8inc=$(HOME)/v8/11.2/v8/


CXXFLAGS= `pkg-config --cflags --libs glib-2.0` -g   -std=c++17 -DV8_COMPRESS_POINTERS  -DV8_ENABLE_SANDBOX
CXXINCLUDE=`pkg-config --cflags --libs glib-2.0` -g -I. -I$(v8inc) -I$(v8inc)/include -Iejax/include
LDFLAGS=`pkg-config --libs glib-2.0`

#CXX=clang++
all: shell testEjax shell-old hello-world

shell: shell.o logging.o base-functions.o utils.o
	$(CXX) -g -o $@ $+ -lv8_monolith -L$(v8obj)  -pthread -lreadline $(CXXFLAGS)  -ldl	

#shell-old: shell-old.o
#	$(CXX) -g -o $@ shell-old.o -lv8_monolith -L$(v8obj)  -pthread -lreadline $(CXXFLAGS)   -ldl

hello-world: hello-world.o
	$(CXX) -g -o $@ $+ -lv8_monolith -L$(v8obj)  -pthread -lreadline $(CXXFLAGS)   -ldl

clean:
	rm -f shell testEjax *.o 

shell.o: shell.cc base-functions.h utils.h
	$(CXX) -g -c -I. -I$(v8inc) -I$(v8inc)/include -Iejax/include $(CXXFLAGS)  $<  

utils.o: utils.cc utils.h
	$(CXX) -g -c -I. -I$(v8inc) -I$(v8inc)/include -Iejax/include $(CXXFLAGS) $<   

base-functions.o: base-functions.cc  base-functions.h utils.h
	$(CXX) -g -c -I. -I$(v8inc) -I$(v8inc)/include -Iejax/include $(CXXFLAGS) $<   

#shell-old.o: shell-old.cc
#	$(CXX) -g -c -I. -I$(v8inc) -I$(v8inc)/include -Iejax/include  $(CXXFLAGS)   $<

hello-world.o: hello-world.cc
	$(CXX) -g -c -I. -I$(v8inc) -I$(v8inc)/include -Iejax/include   $(CXXFLAGS)  $<

logging.o: ejax/logging.cc 
	$(CXX) -g -c -I. -Iejax/include ejax/logging.cc  $(CXXFLAGS) 

text_buffer.o: text_buffer.cc   text_buffer.h
	$(CXX) $(CXXINCLUDE) -g -c $<

text_buffer_view: text_buffer_view.cc   text_buffer_view.h
	$(CXX) $(CXXINCLUDE)  -g -c $<
test_main.o: test_main.cc text_buffer.h text_buffer_view.h

testEjax: test_main.o text_buffer.o text_buffer_view.o logging.o
	$(CXX) $(LDFLAGS) -g  -Iejax/include -o testEjax  $+

testEjax.old: test_main.cc text_buffer.cc   text_buffer.h
	$(CXX) -Iejax/include -o testEjax test_main.cc text_buffer.cc logging.o

ejax-cmd: ejax_cmdline.o ejax_cmdline_process.o
	$(CXX) $(LDFLAGS) -g -o $@ $+  -L./ejax -l ejax -l readline

ejax_cmdline.o: ejax_cmdline.c

ejax_cmdline_process.o: ejax_cmdline_process.c


#
#


cpp-testbed: cpp-testbed.cc

