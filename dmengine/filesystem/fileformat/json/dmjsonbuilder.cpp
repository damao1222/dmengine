/*
   Copyright (C) 2012-2014 Xiongfa Li, <damao1222@live.com>
   All rights reserved.

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/

#include "dmjsonbuilder.h"
#include "dmvector.h"
#include "dmvariant.h"
#include "dmutilstring.h"
#include "dmjsonwriter.h"

DM_BEGIN_NAMESPACE
class JsonBuilderPrivate
{
public:
	JsonBuilderPrivate();
	void cleanup();
	dbool save(const char *fileName);

	void beginObject();
	void insertName(const UtilString &name);
	void insertValue(const Variant &value);
	void endObject();

	void beginArray();
	void addChild(const Variant &value);
	void endArray();

	const Variant& root() const { return m_root; }

	static dbool write(const char *fileName, const Variant &root);
private:
	void addProperty(const UtilString &name, const Variant &value);
	void push(const Variant &v);
	Variant& last();
	Variant pop();

	void pushName(const UtilString &v);
	UtilString& lastName();
	UtilString popName();

	Variant m_root;
	Vector<Variant> m_stack;
	Vector<UtilString> m_nameStack;
};

JsonBuilderPrivate::JsonBuilderPrivate()
{
//    push(m_root);
}

void JsonBuilderPrivate::cleanup()
{
    m_nameStack.clear();
    m_stack.clear();
    m_root.clear();
}

dbool JsonBuilderPrivate::write(const char *fileName, const Variant &root)
{
    JsonWriter writer;
    writer.write(fileName, root);
    return true;
}

dbool JsonBuilderPrivate::save(const char *fileName)
{
    return write(fileName, m_root);
}

void JsonBuilderPrivate::beginObject()
{
    Variant v = VarHash();
    push(v);
}

void JsonBuilderPrivate::insertName(const UtilString &name)
{
    pushName(name);
    //FIXME: add a null object, this is neccessary?
    last().insert(name, Variant());
}

void JsonBuilderPrivate::insertValue(const Variant &value)
{
    const UtilString &name = popName();
    DM_ASSERT(!name.isEmpty());
    last().insert(name, value);
}

void JsonBuilderPrivate::addProperty(const UtilString &name, const Variant &value)
{
    DM_ASSERT(!name.isEmpty());
    last().insert(name, value);
}

void JsonBuilderPrivate::endObject()
{
    Variant v = pop();
    if (!m_stack.isEmpty())
    {
        if (last().isList())
        {
            addChild(v);
        }
        else if (last().isHash())
        {
            insertValue(v);
        }
    }
    else
    {
        m_root = v;
    }
}

void JsonBuilderPrivate::beginArray()
{
    Variant v = VarList();
    push(v);
}

void JsonBuilderPrivate::addChild(const Variant &v)
{
    last().append(v);
}

void JsonBuilderPrivate::endArray()
{
    Variant v = pop();
    if (!m_stack.isEmpty())
    {
        if (last().isList())
        {
            addChild(v);
        }
        else if (last().isHash())
        {
            insertValue(v);
        }
    }
    else
    {
        m_root = v;
    }
}

void JsonBuilderPrivate::push(const Variant &v)
{
    m_stack.append(v);
}

Variant& JsonBuilderPrivate::last()
{
    return m_stack.last();
}

Variant JsonBuilderPrivate::pop()
{
    return m_stack.takeLast();
}

void JsonBuilderPrivate::pushName(const UtilString &v)
{
    m_nameStack.append(v);
}

UtilString& JsonBuilderPrivate::lastName()
{
    return m_nameStack.last();
}

UtilString JsonBuilderPrivate::popName()
{
    return m_nameStack.takeLast();
}

JsonBuilder::JsonBuilder()
	:C_D(JsonBuilder)
{

}

JsonBuilder::~JsonBuilder()
{
	D_D(JsonBuilder);
}

void JsonBuilder::cleanup()
{
	pdm->cleanup();
}

dbool JsonBuilder::save(const char *fileName)
{
	return pdm->save(fileName);
}

void JsonBuilder::beginObject()
{
	pdm->beginObject();
}

void JsonBuilder::insertName(const UtilString &name)
{
	pdm->insertName(name);
}

void JsonBuilder::insertValue(const Variant &value)
{
	pdm->insertValue(value);
}

void JsonBuilder::endObject()
{
	pdm->endObject();
}

void JsonBuilder::beginArray()
{
	pdm->beginArray();
}

void JsonBuilder::addChild(const Variant &value)
{
	pdm->addChild(value);
}

void JsonBuilder::endArray()
{
	pdm->endArray();
}

const Variant& JsonBuilder::root() const
{
	return pdm->root();
}

dbool JsonBuilder::write(const char *fileName, const Variant &root)
{
	return JsonBuilderPrivate::write(fileName, root);
}

DM_END_NAMESPACE