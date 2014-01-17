#$Id: Makefile,v 1.2 2009/07/03 10:30:25 wanghao2 Exp $
COMPILER?=g++
CC:=$(COMPILER)

WORKROOT=/

ifeq ($(MAC),64)
  LIBPATH=$(WORKROOT)lib2-64
  THIRDPATH=$(WORKROOT)third-64
else
  LIBPATH=$(WORKROOT)lib2
  THIRDPATH=$(WORKROOT)third
endif
###############baidu lib 的32位 64位支持#################
 HARDWARE_PLATFORM := $(shell uname -m)
 ifeq ($(HARDWARE_PLATFORM),x86_64)
     lib2=lib2-64
     third=third-64
 else
     lib2=lib2
     third=third
 endif
 ##########################################
#DBPOOL= ../
#DBPOOLLIB=$(DBPOOL)/output/
#ULIB= $(LIBPATH)/ullib/
#BSL=$(LIBPATH)/bsl/
#PUBLIC=$(WORKROOT)/public
#CONFIG=$(PUBLIC)/configure/output
#LOG=$(PUBLIC)/comlog-plugin/output
#MYSQL=$(THIRDPATH)/mysql
#CONNECTPOOL=$(PUBLIC)/connectpool/output
#UBLIB=$(WORKROOT)/public/ub/
#NSHEAD=$(WORKROOT)/public/nshead/
#SPREG=$(WORKROOT)/public/spreg/output/

#INCLUDE_PATH= -I$(SPREG)/include/ -I$(DBPOOL) -I/usr/local/include/mysql/ -I$(ULIB)/include/ -I$(CONFIG)/include -I$(BSL)/include -I$(LOG)/include -I$(MYSQL)/include -I$(CONNECTPOOL)/ -I$(UBLIB)/include -I$(NSHEAD) -I/home/work/server/mysql/include/mysql
#INCLUDE_PATH=-I/home/work/server/mysql/include/mysql
INCLUDE_PATH=-I/home/liyong/include/mysql -I/home/liyong/mongodb/c-driver/src -I/home/liyong/include/hiredis -I/home/work/public/ub/$(lib2)/bsl/include/ -I/home/zhengxie/trunk -I/home/work/public/spreg/output//include/ -I/home/work/public/mcpack/output//include/ -I/home/work/public/comlog-plugin/output//include/ -I/home/work/public/connectpool/output// -I/home/work/public/bslext/output//include/ -I/home/work/public/ubrpc/output//include/ -I/home/work/public/gm/galileo/output//include/ -I/home/work/public/uconv/output//include/ -I/home/work/public/nshead/output//include/ -I/home/work/public/ub/output//include/ -I/home/work/public/myclient/output//include/ -I/home/work/public/configure/output//include/ -I/home/work/public/odict/output//include/ -I/home/work/public/idlcompiler/output//include/ -I/home/work/public/ub/$(lib2)/ullib/include/ -I/home/work//public/ub/$(third)/boost// -I/home/work/public/ub/$(lib2)/bsl/include/ -I/home/work/server/mysql/include/mysql/   -I/home/work/public/ub//$(third)/pcre//include/ -I./include -I/home/work/server/redis-stable/deps/hiredis -I/home/work/server/redis-stable/deps/hiredis/adapters
#INCLUDE_PATH=-I/home/liyong/include/mysql -I/home/liyong/mongodb/c-driver/src
#LIB_PATH= -L$(DBPOOL) -L/usr/local/lib/mysql/ -L$(CONNECTPOOL)/ -L$(CONFIG)/lib -L$(SPREG)/lib/ -L$(BSL)/lib -L$(LOG)/lib -L$(MYSQL)/lib/ -L$(UBLIB)/lib -L$(NSHEAD) -L$(ULIB)/lib/
LIB_PATH= -L. -L/home/liyong/lib/mysql/ -L/home/liyong/mongodb/c-driver -L/home/work/public/hiredis/ -L/home/work/public/connectpool/output//  -L/home/work/public/gm/galileo/output//lib/ -L/home/work/public/idlcompiler/output//lib/ -L/home/work/public/connectpool/output// -L/home/work/public/comlog-plugin/output//lib/ -L/home/work/public/nshead/output// -L/home/work/public/ubrpc/output//lib/ -L/home/work/public/bslext/output//lib/  -L/home/work/public/ub/$(lib2)/bsl//lib/ -L/home/work/public/ub/$(lib2)/ullib//lib/  -L/home/work/public/ub/$(lib2)/dict//lib/ -L/home/work/public/mcpack/output//lib/ -L/home/work/public/configure/output//lib/ -L/home/work/public/odict/output// -L/home/work/public/uconv/output// -L/home/work/public/ub/output//lib/ -L/home/work/public/spreg/output//lib/   -L/home/work/public/ub/$(third)/pcre/lib/ -L/home/work/server/lib/ -L/home/work/public/myclient/output//lib/
#LIB_PATH= -L. -L/home/liyong/lib/mysql/ -L/home/liyong/mongodb/c-driver
#LIB=-lmyclient -lconnectpool -lconfig -lspreg -lbsl -lcomlog -lub -lnshead -lullib -lmysqlclient -lpthread -lz
LIB=-lmysqlclient -lmongoc -lbson -lhiredis -lub -lservlet -lclient -lmyclient -lub -lgalileo -lzookeeper_mt -lcomlog -ldfsappender -lbsl_bml -lbsl_var_serializer -lbsl_var_scripting -lbsl_vs -lbsl_containers_utils -lbsl_var_vscript -lbslext -lconnectpool -lconfig -lmcpack_idl -lodict -luconv -lmcpack -lspreg -lnshead -luldict -lullib -lpcre -lpcrecpp -lpcreposix -lbsl -lmysqlclient -lz libbson.a libmongoc.a -lhiredis -ljson_linux-gcc-3.4.5_libmt -lservlet -lclient -lmyclient -lub -lgalileo -lzookeeper_mt -lcomlog -ldfsappender -lbsl_bml -lbsl_var_serializer -lbsl_var_scripting -lbsl_vs -lbsl_containers_utils -lbsl_var_vscript -lbslext -lconnectpool -lconfig -lmcpack_idl -lodict -luconv -lmcpack -lspreg -lnshead -luldict -lullib -lpcre -lpcrecpp -lpcreposix -lbsl -lmysqlclient -lz libbson.a libmongoc.a -lhiredis -ljson_linux-gcc-3.4.5_libmt
#LIB=-lmysqlclient -lmongoc -lbson


OBJ:=upmongodb update_url upiosmongodb up_online_game upmixedmongodb upstar syncBaiduAppInfo2Mongodb

#CPPFLAGS = -g -Wall -Winline -W -Wreturn-type -Wtrigraphs -Wformat -Wparentheses -Wpointer-arith -Werror -D_GNU_SOURCE
#CPPFLAGS = -g -Wall
#CXXFLAGS= -DMONGODB_HOST="10.23.245.118" -g -Wall $(INCLUDE_PATH) $(LIB_PATH) $(LIB) -DMONGO_HAVE_UNISTD 
CXXFLAGS=-c -ansi -Wall -ggdb -DMONGO_HAVE_STDINT -D_POSIX_SOURCE -DMONGO_DLL_BUILD -DMONGO_HAVE_STDINT $(INCLUDE_PATH)
#CXXFLAGS=-c -ansi -pedantic -Wall -ggdb -DMONGO_HAVE_STDINT -D_POSIX_SOURCE -O3 -DMONGO_DLL_BUILD -DMONGO_HAVE_STDINT $(INCLUDE_PATH)
CPPFLAGS = -g -Wall  $(INCLUDE_PATH) $(LIB_PATH) $(LIB) -DMONGO_HAVE_UNISTD


all: $(OBJ)

upmongodb :  upmongodb.o common.o redis_connpool.o
	$(CC) -o $@ $^  $(CPPFLAGS)

update_url : update_url.o common.o redis_connpool.o

	$(CC) -o $@ $^  $(CPPFLAGS)

upiosmongodb : upiosmongodb.o common.o redis_connpool.o

	$(CC) -o $@ $^  $(CPPFLAGS)

up_online_game : up_online_game.o common.o redis_connpool.o

	$(CC) -o $@ $^  $(CPPFLAGS)

upmixedmongodb : upmixedmongodb.o common.o redis_connpool.o

	$(CC) -o $@ $^  $(CPPFLAGS)

upstar : upstar.o common.o redis_connpool.o

	$(CC) -o $@ $^  $(CPPFLAGS)

syncBaiduAppInfo2Mongodb : syncBaiduAppInfo2Mongodb.o

	$(CC) -o $@ $^  $(CPPFLAGS)
%.o: %.cpp
	$(CC) $(CXXFLAGS)  $< -o $@  

clean:
	rm -f *.o $(OBJ)
