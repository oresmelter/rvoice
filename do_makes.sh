#!/bin/bash

#############
# FUNCTIONS #
#############

# для упрощения
# 1 аргумент - путь к Makefile
# 2 аргумент - запись
function add_record
{
echo -e "$2" >> $1/Makefile
}

# Создание правила для сборки
# верхнего Makefile
function write_top_rule
{
TGT=$1
if [ -z "$TGT" ]
   then
   TGT="all"
fi

add_record . "$TGT:\n\t\t@echo Making $1:"
if [ -n "$SUBDIRS" ]
   then
   add_record . "\t\t@MYPWD=\`pwd\` ; for i in \$(SUBDIRS) ; do cd \$\$i && make $1 ; cd \$\$MYPWD ; done"
fi
if [ -n "$SRC_DIR" ]
   then
   add_record . "\t\t@cd \$(SRC_DIR) && make $1"
fi
add_record . "\t\t@echo Making $1 done.\n"
}

# working rules
function write_working_rule
{
if [ -z "$SRC_DIR" ]
   then
   SRC_DIR=.
fi

echo -n "" > ./$SRC_DIR/Makefile

common="
TARGET=$TARGET

CXX=$CXX
CPP=$CPP
CC=$CC

RM=rm -f
AR=ar r

CFLAGS=$CFLAGS
LDFLAGS=$LDFLAGS

CPPSOURCES=$CPPSOURCES
CSOURCES=$CSOURCES

INCLUDES=$INCLUDES
LIBPATHS=$LIBPATHS
LIBRARIES=$LIBRARIES

COBJECTS=\$(CSOURCES:.c=.o)
CPPOBJECTS=\$(CPPSOURCES:.cpp=.o)

LIBS=\$(addprefix -l, \$(LIBRARIES) )
LIBPATH=\$(addprefix -L, \$(LIBPATHS) )
INCLUDE=\$(addprefix -I, \$(INCLUDES) )

"
application="
all: \$(COBJECTS) \$(CPPOBJECTS)
		\$(CXX) -o \$(TARGET) \$(LDFLAGS) \$(COBJECTS) \$(CPPOBJECTS) \$(LIBS) \$(LIBPATH)
"

static="
all: \$(COBJECTS) \$(CPPOBJECTS)
		\$(AR) \$(TARGET) \$(COBJECTS) \$(CPPOBJECTS)
"

dynamic="
all: \$(COBJECTS) \$(CPPOBJECTS)
		\$(CXX) -shared -o \$(TARGET) \$(LDFLAGS) \$(COBJECTS) \$(CPPOBJECTS) \$(LIBS) \$(LIBPATH)
"

builds="
.c.o:
		\$(CC) \$(INCLUDE) \$(CFLAGS) -o \$@ -c \$<

.cpp.o:
		\$(CPP) \$(INCLUDE) \$(CFLAGS) -o \$@ -c \$<
"

clean="
clean:
		\$(RM) \$(TARGET) *.o *.a *.so core core.*

"

add_record ./$SRC_DIR "$common"

if [ "$MODE" == "static" ]
   then
   add_record ./$SRC_DIR "$static"
fi


if [ "$MODE" == "dynamic" ]
   then
   add_record ./$SRC_DIR "$dynamic"
fi

if [ "$MODE" == "application" ]
   then
   add_record ./$SRC_DIR "$application"
fi

if [ "$MODE" == "nobuild" ]
   then
   echo -n ""
   else
   add_record ./$SRC_DIR "$builds"
   add_record ./$SRC_DIR "$clean"
fi

}


####################
# BUILD START HERE #
####################

# Проверка цели
res=`echo "$TARGET"`
if [ -z "$res" ]
   then
   MODE="nobuild"
   else
   MODE="application"
fi

res=`echo "$TARGET" | grep -E "^lib.+\.a$"`
if [ -n "$res" ]
   then
   MODE="static"
   echo "Will create static library $TARGET."
fi

res=`echo "$TARGET" | grep -E "^lib.+\.so$"`
if [ -n "$res" ]
   then
   MODE="dynamic"
   echo "Will create shared library $TARGET."
fi

if [ "$MODE" == "application" ]
   then
   echo "Will create application $TARGET"
fi

# Проверка исходников
MYPWD=`pwd`

cd $SRC_DIR
list=`ls -1`
cd $MYPWD
echo -n "Checking for *.cpp sources: "
cppres=`echo "$list" | grep -E ".*\.cpp$"`
if [ -n "$cppres" ]
   then
   CPP=g++
   CXX=g++
   echo "yes."
   CPPSOURCES=`echo $cppres`
   else
   if [ -z "$CXX" ]
      then
      CXX=gcc
   fi
   CPP=
   CPPSOURCES=
   echo "no."
fi

echo -n "Checking for *.c sources: "
cres=`echo "$list" | grep -E ".*\.c$"`
if [ -n "$cres" ]
   then
   CC=gcc
   echo "yes."
   CSOURCES=`echo $cres`
   else
   CC=
   CSOURCES=
   echo "no."
fi

# Обрабатываем подкаталоги
if [ -n "$SUBDIRS" ]
   then
   for DIR in $SUBDIRS
       do
       echo "Configuring $DIR"
       cd $DIR && ./configure
       cd $MYPWD
       done
fi

# Создаем Makefiles
echo -n "Creating Makefile:"
echo "
SRC_DIR=$SRC_DIR
SUBDIRS=$SUBDIRS
" > ./Makefile
write_top_rule
write_top_rule clean
echo " done"

if [ "$MODE" == "nobuild" ]
   then
   echo -n ""
   else
   # если рабочая папка совпадает с корневой
   # просто перезаписываем Makefile.
   echo -n "Creating $SRC_DIR/Makefile: "
   write_working_rule
fi

echo "done"

