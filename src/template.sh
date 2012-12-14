#!/bin/bash

if [ -z $1 ]; then
    echo "Usage: template [class_name]"
else

echo "#ifndef $1_H
#define $1_H

class $1
{
	//params
public:
	$1();
	~$1();
};

#endif" > $1.h

echo "#include \"$1.h\"

$1::$1()
{
}

$1::~$1()
{
}" > $1.cpp

fi
