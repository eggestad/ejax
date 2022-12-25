
v8obj=$(HOME)/v8/v8/out.gn/x64.release.sample/obj
v8inc=$(HOME)/v8/v8/

#CXX=clang++
all: shell testEjax shell-old hello-world

shell: shell.o logging.o base-functions.o utils.o
	$(CXX) -g -o $@ $+ -lv8_monolith -L$(v8obj)  -pthread -lreadline -std=c++14 -DV8_COMPRESS_POINTERS -ldl	

shell-old: shell-old.o
	$(CXX) -g -o $@ shell-old.o -lv8_monolith -L$(v8obj)  -pthread -lreadline -std=c++14 -DV8_COMPRESS_POINTERS -ldl

hello-world: hello-world.o
	$(CXX) -g -o $@ $+ -lv8_monolith -L$(v8obj)  -pthread -lreadline -std=c++14 -DV8_COMPRESS_POINTERS -ldl

clean:
	rm -f shell testEjax *.o 

shell.o: shell.cc base-functions.h utils.h
	$(CXX) -g -c -I. -I$(v8inc) -I$(v8inc)/include -Iejax/include $< -std=c++14 -DV8_COMPRESS_POINTERS 

utils.o: utils.cc utils.h
	$(CXX) -g -c -I. -I$(v8inc) -I$(v8inc)/include -Iejax/include $< -std=c++14 -DV8_COMPRESS_POINTERS 

base-functions.o: base-functions.cc  base-functions.h utils.h
	$(CXX) -g -c -I. -I$(v8inc) -I$(v8inc)/include -Iejax/include $< -std=c++14 -DV8_COMPRESS_POINTERS 

shell-old.o: shell-old.cc 
	$(CXX) -g -c -I. -I$(v8inc) -I$(v8inc)/include -Iejax/include  -std=c++14 -DV8_COMPRESS_POINTERS  $<

hello-world.o: hello-world.cc
	$(CXX) -g -c -I. -I$(v8inc) -I$(v8inc)/include -Iejax/include  -std=c++14 -DV8_COMPRESS_POINTERS  $<

logging.o: ejax/logging.cc 
	$(CXX) -g -c -I. -Iejax/include ejax/logging.cc -std=c++14 -DV8_COMPRESS_POINTERS 

testEjax: test_main.cc text_buffer.cc   text_buffer.h
	$(CXX) -Iejax/include -o testEjax test_main.cc text_buffer.cc logging.o
