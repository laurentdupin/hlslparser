//#include "Engine/Assert.h"
#include "Engine.h"

#include "HLSLTree.h"

namespace M4
{
    nlohmann::json      HLSLType::ConvertToJSON()
    {
        nlohmann::json output = nlohmann::json::object();

        output["baseType"] = magic_enum::enum_name(baseType);
        output["samplerType"] = magic_enum::enum_name(samplerType);
        if (typeName != NULL) output["typeName"] = typeName;
        output["array"] = array;
        if (arraySize != NULL) output["arraySize"] = arraySize->ConvertToJSON();
        if(flags != 0) output["flags"] = flags;
        if(addressSpace != HLSLAddressSpace::Undefined) output["addressSpace"] = magic_enum::enum_name(addressSpace);

        return output;
    }

    nlohmann::json      HLSLNode::ConvertToJSON(bool bNodeType)
    {
        nlohmann::json output = nlohmann::json::object();
        if(bNodeType) output["nodeType"] = magic_enum::enum_name(nodeType);

        /*if (fileName != NULL)
        {
            output["fileName"] = fileName;
            output["line"] = line;
        }*/

        return output;
    }

    nlohmann::json      HLSLRoot::ConvertToJSON(bool bNodeType)
    {
        nlohmann::json output = HLSLNode::ConvertToJSON(bNodeType);

        output["statements"] = nlohmann::json::array();

        HLSLStatement* nextstatement = statement;

        while (nextstatement != NULL)
        {
            output["attributes"].push_back(nextstatement->ConvertToJSON());
            nextstatement = nextstatement->nextStatement;
        }

        return output;
    }

    nlohmann::json      HLSLStatement::ConvertToJSON(bool bNodeType)

    {
        nlohmann::json output = HLSLNode::ConvertToJSON(bNodeType);
        //output["hidden"] = hidden;
       
        HLSLAttribute* nextattribute = attributes;
        if(nextattribute != NULL) 
            output["attributes"] = nlohmann::json::array();

        while (nextattribute != NULL)
        {
            output["attributes"].push_back(nextattribute->ConvertToJSON());
            nextattribute = nextattribute->nextAttribute;
        }

        return output;
    }

    nlohmann::json      HLSLAttribute::ConvertToJSON(bool bNodeType)
    {
        nlohmann::json output = HLSLNode::ConvertToJSON(bNodeType);

        output["attributeType"] = magic_enum::enum_name(attributeType);
        output["arguments"] = nlohmann::json::array();

        HLSLExpression* nextargument = argument;

        while (nextargument != NULL)
        {
            output["arguments"].push_back(nextargument->ConvertToJSON());
            nextargument = nextargument->nextExpression;
        }

        return output;
    }

    nlohmann::json      HLSLDeclaration::ConvertToJSON(bool bNodeType)
    {
        nlohmann::json output = HLSLStatement::ConvertToJSON(bNodeType);

        if (name != NULL) output["name"] = name;
        output["type"] = type.ConvertToJSON();
        if (registerName != NULL) output["registerName"] = registerName;
        if (semantic != NULL) output["semantic"] = semantic;

        output["assignments"] = nlohmann::json::array();
        HLSLExpression* nextassignment = assignment;

        while (nextassignment != NULL)
        {
            output["assignments"].push_back(nextassignment->ConvertToJSON());
            nextassignment = nextassignment->nextExpression;
        }

        return output;
    }

    nlohmann::json      HLSLStruct::ConvertToJSON(bool bNodeType)
    {
        nlohmann::json output = HLSLStatement::ConvertToJSON(bNodeType);

        if (name != NULL) output["name"] = name;

        output["fields"] = nlohmann::json::array();
        HLSLStructField* nextfield = field;

        while (nextfield != NULL)
        {
            output["fields"].push_back(nextfield->ConvertToJSON(false));
            nextfield = nextfield->nextField;
        }

        return output;
    }

    nlohmann::json      HLSLStructField::ConvertToJSON(bool bNodeType)
    {
        nlohmann::json output = HLSLNode::ConvertToJSON(bNodeType);

        if (name != NULL) output["name"] = name;
        output["type"] = type.ConvertToJSON();
        if (semantic != NULL) output["semantic"] = semantic;
        if (sv_semantic != NULL) output["sv_semantic"] = sv_semantic;

        //output["hidden"] = hidden;

        return output;
    }

    nlohmann::json      HLSLBuffer::ConvertToJSON(bool bNodeType)
    {
        nlohmann::json output = HLSLStatement::ConvertToJSON(bNodeType);

        if (name != NULL) output["name"] = name;
        if (registerName != NULL) output["registerName"] = registerName;
        if (spaceName != NULL) output["spaceName"] = spaceName;

        output["fields"] = nlohmann::json::array();
        HLSLDeclaration* nextdeclaration = field;

        while (nextdeclaration != NULL)
        {
            output["fields"].push_back(nextdeclaration->ConvertToJSON());
            nextdeclaration = nextdeclaration->nextDeclaration;
        }

        return output;
    }

    nlohmann::json      HLSLFunction::ConvertToJSON(bool bNodeType)
    {
        nlohmann::json output = HLSLStatement::ConvertToJSON(bNodeType);

        if (name != NULL) output["name"] = name;
        output["returnType"] = returnType.ConvertToJSON();
        if (semantic != NULL) output["semantic"] = semantic;
        if (sv_semantic != NULL) output["sv_semantic"] = sv_semantic;
        //output["numArguments"] = numArguments;
        //output["numOutputArguments"] = numOutputArguments;

        output["arguments"] = nlohmann::json::array();
        HLSLArgument* nextargument = argument;

        while (nextargument != NULL)
        {
            output["arguments"].push_back(nextargument->ConvertToJSON(false));
            nextargument = nextargument->nextArgument;
        }

        /*output["statements"] = nlohmann::json::array();
        HLSLStatement* nextstatement = statement;

        while (nextstatement != NULL)
        {
            output["statements"].push_back(nextstatement->ConvertToJSON());
            nextstatement = nextstatement->nextStatement;
        }*/

        if (forward != NULL) output["forward"] = forward->ConvertToJSON();

        return output;
    }

    nlohmann::json      HLSLArgument::ConvertToJSON(bool bNodeType)
    {
        nlohmann::json output = HLSLNode::ConvertToJSON(bNodeType);

        if (name != NULL) output["name"] = name;
        output["modifier"] = magic_enum::enum_name(modifier);
        output["type"] = type.ConvertToJSON();
        if (semantic != NULL) output["semantic"] = semantic;
        if (sv_semantic != NULL) output["sv_semantic"] = sv_semantic;

        output["defaultValue"] = nlohmann::json::array();
        HLSLExpression* nextexpression = defaultValue;

        while (nextexpression != NULL)
        {
            output["defaultValue"].push_back(nextexpression->ConvertToJSON());
            nextexpression = nextexpression->nextExpression;
        }

        //output["hidden"] = hidden;

        return output;
    }

    nlohmann::json      HLSLExpression::ConvertToJSON(bool bNodeType)
    {
        nlohmann::json output = HLSLNode::ConvertToJSON(bNodeType);

        output["type"] = expressionType.ConvertToJSON();

        return output;
    }

const HLSLTypeDimension BaseTypeDimension[(int)HLSLBaseType::Count] =
{
    HLSLTypeDimension::None,     // HLSLBaseType::Unknown,
    HLSLTypeDimension::None,     // HLSLBaseType::Void,
    HLSLTypeDimension::Scalar,   // HLSLBaseType::Float,
    HLSLTypeDimension::Vector2,  // HLSLBaseType::Float2,
    HLSLTypeDimension::Vector3,  // HLSLBaseType::Float3,
    HLSLTypeDimension::Vector4,  // HLSLBaseType::Float4,
    HLSLTypeDimension::Matrix2x2,// HLSLBaseType::Float2x2,
    HLSLTypeDimension::Matrix3x3,// HLSLBaseType::Float3x3,
    HLSLTypeDimension::Matrix4x4,// HLSLBaseType::Float4x4,
    HLSLTypeDimension::Matrix4x3,// HLSLBaseType::Float4x3,
    HLSLTypeDimension::Matrix4x2,// HLSLBaseType::Float4x2,
    HLSLTypeDimension::Scalar,   // HLSLBaseType::Half,
    HLSLTypeDimension::Vector2,  // HLSLBaseType::Half2,
    HLSLTypeDimension::Vector3,  // HLSLBaseType::Half3,
    HLSLTypeDimension::Vector4,  // HLSLBaseType::Half4,
    HLSLTypeDimension::Matrix2x2,// HLSLBaseType::Half2x2,
    HLSLTypeDimension::Matrix3x3,// HLSLBaseType::Half3x3,
    HLSLTypeDimension::Matrix4x4,// HLSLBaseType::Half4x4,
    HLSLTypeDimension::Matrix4x3,// HLSLBaseType::Half4x3,
    HLSLTypeDimension::Matrix4x2,// HLSLBaseType::Half4x2,
    HLSLTypeDimension::Scalar,   // HLSLBaseType::Bool,
    HLSLTypeDimension::Vector2,  // HLSLBaseType::Bool2,
    HLSLTypeDimension::Vector3,  // HLSLBaseType::Bool3,
    HLSLTypeDimension::Vector4,  // HLSLBaseType::Bool4,
    HLSLTypeDimension::Scalar,   // HLSLBaseType::Int,
    HLSLTypeDimension::Vector2,  // HLSLBaseType::Int2,
    HLSLTypeDimension::Vector3,  // HLSLBaseType::Int3,
    HLSLTypeDimension::Vector4,  // HLSLBaseType::Int4,
    HLSLTypeDimension::Scalar,   // HLSLBaseType::Uint,
    HLSLTypeDimension::Vector2,  // HLSLBaseType::Uint2,
    HLSLTypeDimension::Vector3,  // HLSLBaseType::Uint3,
    HLSLTypeDimension::Vector4,  // HLSLBaseType::Uint4,
    HLSLTypeDimension::None,     // HLSLBaseType::Texture,
    HLSLTypeDimension::None,     // HLSLBaseType::Sampler,           // @@ use type inference to determine sampler type.
    HLSLTypeDimension::None,     // HLSLBaseType::Sampler2D,
    HLSLTypeDimension::None,     // HLSLBaseType::Sampler3D,
    HLSLTypeDimension::None,     // HLSLBaseType::SamplerCube,
    HLSLTypeDimension::None,     // HLSLBaseType::Sampler2DShadow,
    HLSLTypeDimension::None,     // HLSLBaseType::Sampler2DMS,
    HLSLTypeDimension::None,     // HLSLBaseType::Sampler2DArray,
    HLSLTypeDimension::None,     // HLSLBaseType::UserDefined,       // struct
    HLSLTypeDimension::None,     // HLSLBaseType::Expression,        // type argument for defined() sizeof() and typeof().
    HLSLTypeDimension::None,     // HLSLBaseType::Auto,
};

const HLSLBaseType ScalarBaseType[(int)HLSLBaseType::Count] = {
    HLSLBaseType::Unknown,       // HLSLBaseType::Unknown,
    HLSLBaseType::Void,          // HLSLBaseType::Void,
    HLSLBaseType::Float,         // HLSLBaseType::Float,
    HLSLBaseType::Float,         // HLSLBaseType::Float2,
    HLSLBaseType::Float,         // HLSLBaseType::Float3,
    HLSLBaseType::Float,         // HLSLBaseType::Float4,
    HLSLBaseType::Float,         // HLSLBaseType::Float2x2,
    HLSLBaseType::Float,         // HLSLBaseType::Float3x3,
    HLSLBaseType::Float,         // HLSLBaseType::Float4x4,
    HLSLBaseType::Float,         // HLSLBaseType::Float4x3,
    HLSLBaseType::Float,         // HLSLBaseType::Float4x2,
    HLSLBaseType::Half,          // HLSLBaseType::Half,
    HLSLBaseType::Half,          // HLSLBaseType::Half2,
    HLSLBaseType::Half,          // HLSLBaseType::Half3,
    HLSLBaseType::Half,          // HLSLBaseType::Half4,
    HLSLBaseType::Half,          // HLSLBaseType::Half2x2,
    HLSLBaseType::Half,          // HLSLBaseType::Half3x3,
    HLSLBaseType::Half,          // HLSLBaseType::Half4x4,
    HLSLBaseType::Half,          // HLSLBaseType::Half4x3,
    HLSLBaseType::Half,          // HLSLBaseType::Half4x2,
    HLSLBaseType::Bool,          // HLSLBaseType::Bool,
    HLSLBaseType::Bool,          // HLSLBaseType::Bool2,
    HLSLBaseType::Bool,          // HLSLBaseType::Bool3,
    HLSLBaseType::Bool,          // HLSLBaseType::Bool4,
    HLSLBaseType::Int,           // HLSLBaseType::Int,
    HLSLBaseType::Int,           // HLSLBaseType::Int2,
    HLSLBaseType::Int,           // HLSLBaseType::Int3,
    HLSLBaseType::Int,           // HLSLBaseType::Int4,
    HLSLBaseType::Uint,          // HLSLBaseType::Uint,
    HLSLBaseType::Uint,          // HLSLBaseType::Uint2,
    HLSLBaseType::Uint,          // HLSLBaseType::Uint3,
    HLSLBaseType::Uint,          // HLSLBaseType::Uint4,
    HLSLBaseType::Unknown,       // HLSLBaseType::Texture,
    HLSLBaseType::Unknown,       // HLSLBaseType::Sampler,           // @@ use type inference to determine sampler type.
    HLSLBaseType::Unknown,       // HLSLBaseType::Sampler2D,
    HLSLBaseType::Unknown,       // HLSLBaseType::Sampler3D,
    HLSLBaseType::Unknown,       // HLSLBaseType::SamplerCube,
    HLSLBaseType::Unknown,       // HLSLBaseType::Sampler2DShadow,
    HLSLBaseType::Unknown,       // HLSLBaseType::Sampler2DMS,
    HLSLBaseType::Unknown,       // HLSLBaseType::Sampler2DArray,
    HLSLBaseType::Unknown,       // HLSLBaseType::UserDefined,       // struct
    HLSLBaseType::Unknown,       // HLSLBaseType::Expression,        // type argument for defined() sizeof() and typeof().
    HLSLBaseType::Unknown,       // HLSLBaseType::Auto,
};

#define MAX_NODE_PAGES 4096

HLSLTree::HLSLTree() : m_stringPool()
{
    m_NodePages.reserve(MAX_NODE_PAGES);

    m_NodePages.emplace_back();
    m_firstPage         = &m_NodePages.back();

    m_currentPage       = m_firstPage;
    m_currentPageOffset = 0;

    m_root              = AddNode<HLSLRoot>(NULL, 1);
}

void HLSLTree::AllocatePage()
{
    m_NodePages.emplace_back();

    ASSERT(m_NodePages.size() <= MAX_NODE_PAGES);

    NodePage* newPage    = &m_NodePages.back();
    m_currentPage->next  = newPage;
    m_currentPageOffset  = 0;
    m_currentPage        = newPage;
}

const char* HLSLTree::AddString(const char* string)
{   
    return m_stringPool.AddString(string);
}

const char* HLSLTree::AddStringFormat(const char* format, ...)
{
    va_list args;
    va_start(args, format);
    const char * string = m_stringPool.AddStringFormatList(format, args);
    va_end(args);
    return string;
}

bool HLSLTree::GetContainsString(const char* string) const
{
    return m_stringPool.GetContainsString(string);
}

HLSLRoot* HLSLTree::GetRoot() const
{
    return m_root;
}

void* HLSLTree::AllocateMemory(size_t size)
{
    if (m_currentPageOffset + size > s_nodePageSize)
    {
        AllocatePage();
    }
    void* buffer = m_currentPage->buffer + m_currentPageOffset;
    m_currentPageOffset += size;
    return buffer;
}

// @@ This doesn't do any parameter matching. Simply returns the first function with that name.
HLSLFunction * HLSLTree::FindFunction(const char * name)
{
    HLSLStatement * statement = m_root->statement;
    while (statement != NULL)
    {
        if (statement->nodeType == HLSLNodeType::Function)
        {
            HLSLFunction * function = (HLSLFunction *)statement;
            if (String_Equal(name, function->name))
            {
                return function;
            }
        }

        statement = statement->nextStatement;
    }

    return NULL;
}

HLSLDeclaration * HLSLTree::FindGlobalDeclaration(const char * name, HLSLBuffer ** buffer_out/*=NULL*/)
{
    HLSLStatement * statement = m_root->statement;
    while (statement != NULL)
    {
        if (statement->nodeType == HLSLNodeType::Declaration)
        {
            HLSLDeclaration * declaration = (HLSLDeclaration *)statement;
            if (String_Equal(name, declaration->name))
            {
                if (buffer_out) *buffer_out = NULL;
                return declaration;
            }
        }
        else if (statement->nodeType == HLSLNodeType::Buffer)
        {
            HLSLBuffer* buffer = (HLSLBuffer*)statement;

            HLSLDeclaration* field = buffer->field;
            while (field != NULL)
            {
                ASSERT(field->nodeType == HLSLNodeType::Declaration);
                if (String_Equal(name, field->name))
                {
                    if (buffer_out) *buffer_out = buffer;
                    return field;
                }
                field = (HLSLDeclaration*)field->nextStatement;
            }
        }

        statement = statement->nextStatement;
    }

    if (buffer_out) *buffer_out = NULL;
    return NULL;
}

HLSLStruct * HLSLTree::FindGlobalStruct(const char * name)
{
    HLSLStatement * statement = m_root->statement;
    while (statement != NULL)
    {
        if (statement->nodeType == HLSLNodeType::Struct)
        {
            HLSLStruct * declaration = (HLSLStruct *)statement;
            if (String_Equal(name, declaration->name))
            {
                return declaration;
            }
        }

        statement = statement->nextStatement;
    }

    return NULL;
}

HLSLTechnique * HLSLTree::FindTechnique(const char * name)
{
    HLSLStatement * statement = m_root->statement;
    while (statement != NULL)
    {
        if (statement->nodeType == HLSLNodeType::Technique)
        {
            HLSLTechnique * technique = (HLSLTechnique *)statement;
            if (String_Equal(name, technique->name))
            {
                return technique;
            }
        }

        statement = statement->nextStatement;
    }

    return NULL;
}

HLSLPipeline * HLSLTree::FindFirstPipeline()
{
    return FindNextPipeline(NULL);
}

HLSLPipeline * HLSLTree::FindNextPipeline(HLSLPipeline * current)
{
    HLSLStatement * statement = current ? current : m_root->statement;
    while (statement != NULL)
    {
        if (statement->nodeType == HLSLNodeType::Pipeline)
        {
            return (HLSLPipeline *)statement;
        }

        statement = statement->nextStatement;
    }

    return NULL;
}

HLSLPipeline * HLSLTree::FindPipeline(const char * name)
{
    HLSLStatement * statement = m_root->statement;
    while (statement != NULL)
    {
        if (statement->nodeType == HLSLNodeType::Pipeline)
        {
            HLSLPipeline * pipeline = (HLSLPipeline *)statement;
            if (String_Equal(name, pipeline->name))
            {
                return pipeline;
            }
        }

        statement = statement->nextStatement;
    }

    return NULL;
}

HLSLBuffer * HLSLTree::FindBuffer(const char * name)
{
    HLSLStatement * statement = m_root->statement;
    while (statement != NULL)
    {
        if (statement->nodeType == HLSLNodeType::Buffer)
        {
            HLSLBuffer * buffer = (HLSLBuffer *)statement;
            if (String_Equal(name, buffer->name))
            {
                return buffer;
            }
        }

        statement = statement->nextStatement;
    }

    return NULL;
}



bool HLSLTree::GetExpressionValue(HLSLExpression * expression, int & value)
{
    ASSERT (expression != NULL);

    // Expression must be constant.
    if ((expression->expressionType.flags & (int)HLSLTypeFlags::Const) == 0)
    {
        return false;
    }

    // We are expecting an integer scalar. @@ Add support for type conversion from other scalar types.
    if (expression->expressionType.baseType != HLSLBaseType::Int &&
        expression->expressionType.baseType != HLSLBaseType::Bool)
    {
        return false;
    }

    if (expression->expressionType.array) 
    {
        return false;
    }

    if (expression->nodeType == HLSLNodeType::BinaryExpression) 
    {
        HLSLBinaryExpression * binaryExpression = (HLSLBinaryExpression *)expression;

        int value1, value2;
        if (!GetExpressionValue(binaryExpression->expression1, value1) ||
            !GetExpressionValue(binaryExpression->expression2, value2))
        {
            return false;
        }

        switch(binaryExpression->binaryOp)
        {
            case HLSLBinaryOp::And:
                value = value1 && value2;
                return true;
            case HLSLBinaryOp::Or:
                value = value1 || value2;
                return true;
            case HLSLBinaryOp::Add:
                value = value1 + value2;
                return true;
            case HLSLBinaryOp::Sub:
                value = value1 - value2;
                return true;
            case HLSLBinaryOp::Mul:
                value = value1 * value2;
                return true;
            case HLSLBinaryOp::Div:
                value = value1 / value2;
                return true;
            case HLSLBinaryOp::Less:
                value = value1 < value2;
                return true;
            case HLSLBinaryOp::Greater:
                value = value1 > value2;
                return true;
            case HLSLBinaryOp::LessEqual:
                value = value1 <= value2;
                return true;
            case HLSLBinaryOp::GreaterEqual:
                value = value1 >= value2;
                return true;
            case HLSLBinaryOp::Equal:
                value = value1 == value2;
                return true;
            case HLSLBinaryOp::NotEqual:
                value = value1 != value2;
                return true;
            case HLSLBinaryOp::BitAnd:
                value = value1 & value2;
                return true;
            case HLSLBinaryOp::BitOr:
                value = value1 | value2;
                return true;
            case HLSLBinaryOp::BitXor:
                value = value1 ^ value2;
                return true;
            case HLSLBinaryOp::Assign:
            case HLSLBinaryOp::AddAssign:
            case HLSLBinaryOp::SubAssign:
            case HLSLBinaryOp::MulAssign:
            case HLSLBinaryOp::DivAssign:
                // IC: These are not valid on non-constant expressions and should fail earlier when querying expression value.
                return false;
        }
    }
    else if (expression->nodeType == HLSLNodeType::UnaryExpression) 
    {
        HLSLUnaryExpression * unaryExpression = (HLSLUnaryExpression *)expression;

        if (!GetExpressionValue(unaryExpression->expression, value))
        {
            return false;
        }

        switch(unaryExpression->unaryOp)
        {
            case HLSLUnaryOp::Negative:
                value = -value;
                return true;
            case HLSLUnaryOp::Positive:
                // nop.
                return true;
            case HLSLUnaryOp::Not:
                value = !value;
                return true;
            case HLSLUnaryOp::BitNot:
                value = ~value;
                return true;
            case HLSLUnaryOp::PostDecrement:
            case HLSLUnaryOp::PostIncrement:
            case HLSLUnaryOp::PreDecrement:
            case HLSLUnaryOp::PreIncrement:
                // IC: These are not valid on non-constant expressions and should fail earlier when querying expression value.
                return false;
        }
    }
    else if (expression->nodeType == HLSLNodeType::IdentifierExpression)
    {
        HLSLIdentifierExpression * identifier = (HLSLIdentifierExpression *)expression;

        HLSLDeclaration * declaration = FindGlobalDeclaration(identifier->name);
        if (declaration == NULL) 
        {
            return false;
        }
        if ((declaration->type.flags & (int)HLSLTypeFlags::Const) == 0)
        {
            return false;
        }

        return GetExpressionValue(declaration->assignment, value);
    }
    else if (expression->nodeType == HLSLNodeType::LiteralExpression) 
    {
        HLSLLiteralExpression * literal = (HLSLLiteralExpression *)expression;
   
        if (literal->expressionType.baseType == HLSLBaseType::Int) value = literal->iValue;
        else if (literal->expressionType.baseType == HLSLBaseType::Bool) value = (int)literal->bValue;
        else return false;
        
        return true;
    }

    return false;
}

bool HLSLTree::NeedsFunction(const char* name)
{
    // Early out
    if (!GetContainsString(name))
        return false;

    struct NeedsFunctionVisitor: HLSLTreeVisitor
    {
        const char* name;
        bool result;

        virtual void VisitTopLevelStatement(HLSLStatement * node)
        {
            if (!node->hidden)
                HLSLTreeVisitor::VisitTopLevelStatement(node);
        }

        virtual void VisitFunctionCall(HLSLFunctionCall * node)
        {
            result = result || String_Equal(name, node->function->name);

            HLSLTreeVisitor::VisitFunctionCall(node);
        }
    };

    NeedsFunctionVisitor visitor;
    visitor.name = name;
    visitor.result = false;

    visitor.VisitRoot(m_root);

    return visitor.result;
}

int GetVectorDimension(HLSLType & type)
{
    if (type.baseType >= HLSLBaseType::FirstNumeric &&
        type.baseType <= HLSLBaseType::LastNumeric)
    {
        if (type.baseType == HLSLBaseType::Float || type.baseType == HLSLBaseType::Half) return 1;
        if (type.baseType == HLSLBaseType::Float2 || type.baseType == HLSLBaseType::Half2) return 2;
        if (type.baseType == HLSLBaseType::Float3 || type.baseType == HLSLBaseType::Half3) return 3;
        if (type.baseType == HLSLBaseType::Float4 || type.baseType == HLSLBaseType::Half4) return 4;

    }
    return 0;
}

// Returns dimension, 0 if invalid.
int HLSLTree::GetExpressionValue(HLSLExpression * expression, float values[4])
{
    ASSERT (expression != NULL);

    // Expression must be constant.
    if ((expression->expressionType.flags & (int)HLSLTypeFlags::Const) == 0)
    {
        return 0;
    }

    if (expression->expressionType.baseType == HLSLBaseType::Int ||
        expression->expressionType.baseType == HLSLBaseType::Bool)
    {
        int int_value;
        if (GetExpressionValue(expression, int_value)) {
            for (int i = 0; i < 4; i++) values[i] = (float)int_value;   // @@ Warn if conversion is not exact.
            return 1;
        }

        return 0;
    }
    if (expression->expressionType.baseType >= HLSLBaseType::FirstInteger && expression->expressionType.baseType <= HLSLBaseType::LastInteger)
    {
        // @@ Add support for uints?
        // @@ Add support for int vectors?
        return 0;
    }
    if (expression->expressionType.baseType > HLSLBaseType::LastNumeric)
    {
        return 0;
    }

    // @@ Not supported yet, but we may need it?
    if (expression->expressionType.array) 
    {
        return false;
    }

    if (expression->nodeType == HLSLNodeType::BinaryExpression) 
    {
        HLSLBinaryExpression * binaryExpression = (HLSLBinaryExpression *)expression;
        int dim = GetVectorDimension(binaryExpression->expressionType);

        float values1[4], values2[4];
        int dim1 = GetExpressionValue(binaryExpression->expression1, values1);
        int dim2 = GetExpressionValue(binaryExpression->expression2, values2);

        if (dim1 == 0 || dim2 == 0)
        {
            return 0;
        }

        if (dim1 != dim2)
        {
            // Brodacast scalar to vector size.
            if (dim1 == 1)
            {
                for (int i = 1; i < dim2; i++) values1[i] = values1[0];
                dim1 = dim2;
            }
            else if (dim2 == 1)
            {
                for (int i = 1; i < dim1; i++) values2[i] = values2[0];
                dim2 = dim1;
            }
            else
            {
                return 0;
            }
        }
        ASSERT(dim == dim1);

        switch(binaryExpression->binaryOp)
        {
            case HLSLBinaryOp::Add:
                for (int i = 0; i < dim; i++) values[i] = values1[i] + values2[i];
                return dim;
            case HLSLBinaryOp::Sub:
                for (int i = 0; i < dim; i++) values[i] = values1[i] - values2[i];
                return dim;
            case HLSLBinaryOp::Mul:
                for (int i = 0; i < dim; i++) values[i] = values1[i] * values2[i];
                return dim;
            case HLSLBinaryOp::Div:
                for (int i = 0; i < dim; i++) values[i] = values1[i] / values2[i];
                return dim;
            default:
                return 0;
        }
    }
    else if (expression->nodeType == HLSLNodeType::UnaryExpression) 
    {
        HLSLUnaryExpression * unaryExpression = (HLSLUnaryExpression *)expression;
        int dim = GetVectorDimension(unaryExpression->expressionType);

        int dim1 = GetExpressionValue(unaryExpression->expression, values);
        if (dim1 == 0)
        {
            return 0;
        }
        ASSERT(dim == dim1);

        switch(unaryExpression->unaryOp)
        {
            case HLSLUnaryOp::Negative:
                for (int i = 0; i < dim; i++) values[i] = -values[i];
                return dim;
            case HLSLUnaryOp::Positive:
                // nop.
                return dim;
            default:
                return 0;
        }
    }
    else if (expression->nodeType == HLSLNodeType::ConstructorExpression)
    {
        HLSLConstructorExpression * constructor = (HLSLConstructorExpression *)expression;

        int dim = GetVectorDimension(constructor->expressionType);

        int idx = 0;
        HLSLExpression * arg = constructor->argument;
        while (arg != NULL)
        {
            float tmp[4];
            int n = GetExpressionValue(arg, tmp);
            for (int i = 0; i < n; i++) values[idx + i] = tmp[i];
            idx += n;

            arg = arg->nextExpression;
        }
        ASSERT(dim == idx);

        return dim;
    }
    else if (expression->nodeType == HLSLNodeType::IdentifierExpression)
    {
        HLSLIdentifierExpression * identifier = (HLSLIdentifierExpression *)expression;

        HLSLDeclaration * declaration = FindGlobalDeclaration(identifier->name);
        if (declaration == NULL) 
        {
            return 0;
        }
        if ((declaration->type.flags & (int)HLSLTypeFlags::Const) == 0)
        {
            return 0;
        }

        return GetExpressionValue(declaration->assignment, values);
    }
    else if (expression->nodeType == HLSLNodeType::LiteralExpression)
    {
        HLSLLiteralExpression * literal = (HLSLLiteralExpression *)expression;

        if (literal->expressionType.baseType == HLSLBaseType::Float) values[0] = literal->fValue;
        else if (literal->expressionType.baseType == HLSLBaseType::Half) values[0] = literal->fValue;
        else if (literal->expressionType.baseType == HLSLBaseType::Bool) values[0] = literal->bValue;
        else if (literal->expressionType.baseType == HLSLBaseType::Int) values[0] = (float)literal->iValue;  // @@ Warn if conversion is not exact.
        else return 0;

        return 1;
    }

    return 0;
}




void HLSLTreeVisitor::VisitType(HLSLType & type)
{
}

void HLSLTreeVisitor::VisitRoot(HLSLRoot * root)
{
    HLSLStatement * statement = root->statement;
    while (statement != NULL) {
        VisitTopLevelStatement(statement);
        statement = statement->nextStatement;
    }
}

void HLSLTreeVisitor::VisitTopLevelStatement(HLSLStatement * node)
{
    if (node->nodeType == HLSLNodeType::Declaration) {
        VisitDeclaration((HLSLDeclaration *)node);
    }
    else if (node->nodeType == HLSLNodeType::Struct) {
        VisitStruct((HLSLStruct *)node);
    }
    else if (node->nodeType == HLSLNodeType::Buffer) {
        VisitBuffer((HLSLBuffer *)node);
    }
    else if (node->nodeType == HLSLNodeType::Function) {
        VisitFunction((HLSLFunction *)node);
    }
    else if (node->nodeType == HLSLNodeType::Technique) {
        VisitTechnique((HLSLTechnique *)node);
    }
    else if (node->nodeType == HLSLNodeType::Pipeline) {
        VisitPipeline((HLSLPipeline *)node);
    }
    else {
        ASSERT(0);
    }
}

void HLSLTreeVisitor::VisitStatements(HLSLStatement * statement)
{
    while (statement != NULL) {
        VisitStatement(statement);
        statement = statement->nextStatement;
    }
}

void HLSLTreeVisitor::VisitStatement(HLSLStatement * node)
{
    // Function statements
    if (node->nodeType == HLSLNodeType::Declaration) {
        VisitDeclaration((HLSLDeclaration *)node);
    }
    else if (node->nodeType == HLSLNodeType::ExpressionStatement) {
        VisitExpressionStatement((HLSLExpressionStatement *)node);
    }
    else if (node->nodeType == HLSLNodeType::ReturnStatement) {
        VisitReturnStatement((HLSLReturnStatement *)node);
    }
    else if (node->nodeType == HLSLNodeType::DiscardStatement) {
        VisitDiscardStatement((HLSLDiscardStatement *)node);
    }
    else if (node->nodeType == HLSLNodeType::BreakStatement) {
        VisitBreakStatement((HLSLBreakStatement *)node);
    }
    else if (node->nodeType == HLSLNodeType::ContinueStatement) {
        VisitContinueStatement((HLSLContinueStatement *)node);
    }
    else if (node->nodeType == HLSLNodeType::IfStatement) {
        VisitIfStatement((HLSLIfStatement *)node);
    }
    else if (node->nodeType == HLSLNodeType::ForStatement) {
        VisitForStatement((HLSLForStatement *)node);
    }
    else if (node->nodeType == HLSLNodeType::BlockStatement) {
        VisitBlockStatement((HLSLBlockStatement *)node);
    }
    else {
        ASSERT(0);
    }
}

void HLSLTreeVisitor::VisitDeclaration(HLSLDeclaration * node)
{
    VisitType(node->type);
    /*do {
        VisitExpression(node->assignment);
        node = node->nextDeclaration;
    } while (node);*/
    if (node->assignment != NULL) {
        VisitExpression(node->assignment);
    }
    if (node->nextDeclaration != NULL) {
        VisitDeclaration(node->nextDeclaration);
    }
}

void HLSLTreeVisitor::VisitStruct(HLSLStruct * node)
{
    HLSLStructField * field = node->field;
    while (field != NULL) {
        VisitStructField(field);
        field = field->nextField;
    }
}

void HLSLTreeVisitor::VisitStructField(HLSLStructField * node)
{
    VisitType(node->type);
}

void HLSLTreeVisitor::VisitBuffer(HLSLBuffer * node)
{
    HLSLDeclaration * field = node->field;
    while (field != NULL) {
        ASSERT(field->nodeType == HLSLNodeType::Declaration);
        VisitDeclaration(field);
        ASSERT(field->nextDeclaration == NULL);
        field = (HLSLDeclaration *)field->nextStatement;
    }
}

/*void HLSLTreeVisitor::VisitBufferField(HLSLBufferField * node)
{
    VisitType(node->type);
}*/

void HLSLTreeVisitor::VisitFunction(HLSLFunction * node)
{
    VisitType(node->returnType);

    HLSLArgument * argument = node->argument;
    while (argument != NULL) {
        VisitArgument(argument);
        argument = argument->nextArgument;
    }

    VisitStatements(node->statement);
}

void HLSLTreeVisitor::VisitArgument(HLSLArgument * node)
{
    VisitType(node->type);
    if (node->defaultValue != NULL) {
        VisitExpression(node->defaultValue);
    }
}

void HLSLTreeVisitor::VisitExpressionStatement(HLSLExpressionStatement * node)
{
    VisitExpression(node->expression);
}

void HLSLTreeVisitor::VisitExpression(HLSLExpression * node)
{
    VisitType(node->expressionType);

    if (node->nodeType == HLSLNodeType::UnaryExpression) {
        VisitUnaryExpression((HLSLUnaryExpression *)node);
    }
    else if (node->nodeType == HLSLNodeType::BinaryExpression) {
        VisitBinaryExpression((HLSLBinaryExpression *)node);
    }
    else if (node->nodeType == HLSLNodeType::ConditionalExpression) {
        VisitConditionalExpression((HLSLConditionalExpression *)node);
    }
    else if (node->nodeType == HLSLNodeType::CastingExpression) {
        VisitCastingExpression((HLSLCastingExpression *)node);
    }
    else if (node->nodeType == HLSLNodeType::LiteralExpression) {
        VisitLiteralExpression((HLSLLiteralExpression *)node);
    }
    else if (node->nodeType == HLSLNodeType::IdentifierExpression) {
        VisitIdentifierExpression((HLSLIdentifierExpression *)node);
    }
    else if (node->nodeType == HLSLNodeType::ConstructorExpression) {
        VisitConstructorExpression((HLSLConstructorExpression *)node);
    }
    else if (node->nodeType == HLSLNodeType::MemberAccess) {
        VisitMemberAccess((HLSLMemberAccess *)node);
    }
    else if (node->nodeType == HLSLNodeType::ArrayAccess) {
        VisitArrayAccess((HLSLArrayAccess *)node);
    }
    else if (node->nodeType == HLSLNodeType::FunctionCall) {
        VisitFunctionCall((HLSLFunctionCall *)node);
    }
    // Acoget-TODO: This was missing. Did adding it break anything?
    else if (node->nodeType == HLSLNodeType::SamplerState) {
        VisitSamplerState((HLSLSamplerState *)node);
    }
    else {
        ASSERT(0);
    }
}

void HLSLTreeVisitor::VisitReturnStatement(HLSLReturnStatement * node)
{
    VisitExpression(node->expression);
}

void HLSLTreeVisitor::VisitDiscardStatement(HLSLDiscardStatement * node) {}
void HLSLTreeVisitor::VisitBreakStatement(HLSLBreakStatement * node) {}
void HLSLTreeVisitor::VisitContinueStatement(HLSLContinueStatement * node) {}

void HLSLTreeVisitor::VisitIfStatement(HLSLIfStatement * node)
{
    VisitExpression(node->condition);
    VisitStatements(node->statement);
    if (node->elseStatement) {
        VisitStatements(node->elseStatement);
    }
}

void HLSLTreeVisitor::VisitForStatement(HLSLForStatement * node)
{
    if (node->initialization) {
        VisitDeclaration(node->initialization);
    }
    if (node->condition) {
        VisitExpression(node->condition);
    }
    if (node->increment) {
        VisitExpression(node->increment);
    }
    VisitStatements(node->statement);
}

void HLSLTreeVisitor::VisitBlockStatement(HLSLBlockStatement * node)
{
    VisitStatements(node->statement);
}

void HLSLTreeVisitor::VisitUnaryExpression(HLSLUnaryExpression * node)
{
    VisitExpression(node->expression);
}

void HLSLTreeVisitor::VisitBinaryExpression(HLSLBinaryExpression * node)
{
    VisitExpression(node->expression1);
    VisitExpression(node->expression2);
}

void HLSLTreeVisitor::VisitConditionalExpression(HLSLConditionalExpression * node)
{
    VisitExpression(node->condition);
    VisitExpression(node->falseExpression);
    VisitExpression(node->trueExpression);
}

void HLSLTreeVisitor::VisitCastingExpression(HLSLCastingExpression * node)
{
    VisitType(node->type);
    VisitExpression(node->expression);
}

void HLSLTreeVisitor::VisitLiteralExpression(HLSLLiteralExpression * node) {}
void HLSLTreeVisitor::VisitIdentifierExpression(HLSLIdentifierExpression * node) {}

void HLSLTreeVisitor::VisitConstructorExpression(HLSLConstructorExpression * node)
{
    HLSLExpression * argument = node->argument;
    while (argument != NULL) {
        VisitExpression(argument);
        argument = argument->nextExpression;
    }
}

void HLSLTreeVisitor::VisitMemberAccess(HLSLMemberAccess * node)
{
    VisitExpression(node->object);
}

void HLSLTreeVisitor::VisitArrayAccess(HLSLArrayAccess * node)
{
    VisitExpression(node->array);
    VisitExpression(node->index);
}

void HLSLTreeVisitor::VisitFunctionCall(HLSLFunctionCall * node)
{
    HLSLExpression * argument = node->argument;
    while (argument != NULL) {
        VisitExpression(argument);
        argument = argument->nextExpression;
    }
}

void HLSLTreeVisitor::VisitStateAssignment(HLSLStateAssignment * node) {}

void HLSLTreeVisitor::VisitSamplerState(HLSLSamplerState * node)
{
    HLSLStateAssignment * stateAssignment = node->stateAssignments;
    while (stateAssignment != NULL) {
        VisitStateAssignment(stateAssignment);
        stateAssignment = stateAssignment->nextStateAssignment;
    }
}

void HLSLTreeVisitor::VisitPass(HLSLPass * node)
{
    HLSLStateAssignment * stateAssignment = node->stateAssignments;
    while (stateAssignment != NULL) {
        VisitStateAssignment(stateAssignment);
        stateAssignment = stateAssignment->nextStateAssignment;
    }
}

void HLSLTreeVisitor::VisitTechnique(HLSLTechnique * node)
{
    HLSLPass * pass = node->passes;
    while (pass != NULL) {
        VisitPass(pass);
        pass = pass->nextPass;
    }
}

void HLSLTreeVisitor::VisitPipeline(HLSLPipeline * node)
{
    // @@ ?
}

void HLSLTreeVisitor::VisitFunctions(HLSLRoot * root)
{
    HLSLStatement * statement = root->statement;
    while (statement != NULL) {
        if (statement->nodeType == HLSLNodeType::Function) {
            VisitFunction((HLSLFunction *)statement);
        }

        statement = statement->nextStatement;
    }
}

void HLSLTreeVisitor::VisitParameters(HLSLRoot * root)
{
    HLSLStatement * statement = root->statement;
    while (statement != NULL) {
        if (statement->nodeType == HLSLNodeType::Declaration) {
            VisitDeclaration((HLSLDeclaration *)statement);
        }

        statement = statement->nextStatement;
    }
}


class ResetHiddenFlagVisitor : public HLSLTreeVisitor
{
public:
    virtual void VisitTopLevelStatement(HLSLStatement * statement)
    {
        statement->hidden = true;

        if (statement->nodeType == HLSLNodeType::Buffer)
        {
            VisitBuffer((HLSLBuffer*)statement);
        }
    }

    // Hide buffer fields.
    virtual void VisitDeclaration(HLSLDeclaration * node)
    {
        node->hidden = true;
    }

    virtual void VisitArgument(HLSLArgument * node)
    {
        node->hidden = false;   // Arguments are visible by default.
    }
};

class MarkVisibleStatementsVisitor : public HLSLTreeVisitor
{
public:
    HLSLTree * tree;
    MarkVisibleStatementsVisitor(HLSLTree * tree) : tree(tree) {}

    virtual void VisitFunction(HLSLFunction * node)
    {
        node->hidden = false;
        HLSLTreeVisitor::VisitFunction(node);

        if (node->forward)
            VisitFunction(node->forward);
    }

    virtual void VisitFunctionCall(HLSLFunctionCall * node)
    {
        HLSLTreeVisitor::VisitFunctionCall(node);

        if (node->function->hidden)
        {
            VisitFunction(const_cast<HLSLFunction*>(node->function));
        }
    }

    virtual void VisitIdentifierExpression(HLSLIdentifierExpression * node)
    {
        HLSLTreeVisitor::VisitIdentifierExpression(node);

        if (node->global)
        {
            HLSLDeclaration * declaration = tree->FindGlobalDeclaration(node->name);
            if (declaration != NULL && declaration->hidden)
            {
                declaration->hidden = false;
                VisitDeclaration(declaration);
            }
        }
    }

    virtual void VisitType(HLSLType & type)
    {
        if (type.baseType == HLSLBaseType::UserDefined)
        {
            HLSLStruct * globalStruct = tree->FindGlobalStruct(type.typeName);
            if (globalStruct != NULL)
            {
                globalStruct->hidden = false;
                VisitStruct(globalStruct);
            }
        }
    }

};


void PruneTree(HLSLTree* tree, const char* entryName0, const char* entryName1/*=NULL*/)
{
    HLSLRoot* root = tree->GetRoot();

    // Reset all flags.
    ResetHiddenFlagVisitor reset;
    reset.VisitRoot(root);

    // Mark all the statements necessary for these entrypoints.
    HLSLFunction* entry = tree->FindFunction(entryName0);
    if (entry != NULL)
    {
        MarkVisibleStatementsVisitor mark(tree);
        mark.VisitFunction(entry);
    }

    if (entryName1 != NULL)
    {
        entry = tree->FindFunction(entryName1);
        if (entry != NULL)
        {
            MarkVisibleStatementsVisitor mark(tree);
            mark.VisitFunction(entry);
        }
    }

    // Mark buffers visible, if any of their fields is visible.
    HLSLStatement * statement = root->statement;
    while (statement != NULL)
    {
        if (statement->nodeType == HLSLNodeType::Buffer)
        {
            HLSLBuffer* buffer = (HLSLBuffer*)statement;

            HLSLDeclaration* field = buffer->field;
            while (field != NULL)
            {
                ASSERT(field->nodeType == HLSLNodeType::Declaration);
                if (!field->hidden)
                {
                    buffer->hidden = false;
                    break;
                }
                field = (HLSLDeclaration*)field->nextStatement;
            }
        }

        statement = statement->nextStatement;
    }
}


void SortTree(HLSLTree * tree)
{
    // Stable sort so that statements are in this order:
    // structs, declarations, functions, techniques.
	// but their relative order is preserved.

    HLSLRoot* root = tree->GetRoot();

    HLSLStatement* structs = NULL;
    HLSLStatement* lastStruct = NULL;
    HLSLStatement* constDeclarations = NULL;
    HLSLStatement* lastConstDeclaration = NULL;
    HLSLStatement* declarations = NULL;
    HLSLStatement* lastDeclaration = NULL;
    HLSLStatement* functions = NULL;
    HLSLStatement* lastFunction = NULL;
    HLSLStatement* other = NULL;
    HLSLStatement* lastOther = NULL;

    HLSLStatement* statement = root->statement;
    while (statement != NULL) {
        HLSLStatement* nextStatement = statement->nextStatement;
        statement->nextStatement = NULL;

        if (statement->nodeType == HLSLNodeType::Struct) {
            if (structs == NULL) structs = statement;
            if (lastStruct != NULL) lastStruct->nextStatement = statement;
            lastStruct = statement;
        }
        else if (statement->nodeType == HLSLNodeType::Declaration || statement->nodeType == HLSLNodeType::Buffer) {
            if (statement->nodeType == HLSLNodeType::Declaration && (((HLSLDeclaration *)statement)->type.flags & (int)HLSLTypeFlags::Const)) {
                if (constDeclarations == NULL) constDeclarations = statement;
                if (lastConstDeclaration != NULL) lastConstDeclaration->nextStatement = statement;
                lastConstDeclaration = statement;
            }
            else {
                if (declarations == NULL) declarations = statement;
                if (lastDeclaration != NULL) lastDeclaration->nextStatement = statement;
                lastDeclaration = statement;
            }
        }
        else if (statement->nodeType == HLSLNodeType::Function) {
            if (functions == NULL) functions = statement;
            if (lastFunction != NULL) lastFunction->nextStatement = statement;
            lastFunction = statement;
        }
        else {
            if (other == NULL) other = statement;
            if (lastOther != NULL) lastOther->nextStatement = statement;
            lastOther = statement;
        }

        statement = nextStatement;
    }

    // Chain all the statements in the order that we want.
    HLSLStatement * firstStatement = structs;
    HLSLStatement * lastStatement = lastStruct;

    if (constDeclarations != NULL) {
        if (firstStatement == NULL) firstStatement = constDeclarations;
        else lastStatement->nextStatement = constDeclarations;
        lastStatement = lastConstDeclaration;
    }

    if (declarations != NULL) {
        if (firstStatement == NULL) firstStatement = declarations;
        else lastStatement->nextStatement = declarations;
        lastStatement = lastDeclaration;
    }

    if (functions != NULL) {
        if (firstStatement == NULL) firstStatement = functions;
        else lastStatement->nextStatement = functions;
        lastStatement = lastFunction;
    }

    if (other != NULL) {
        if (firstStatement == NULL) firstStatement = other;
        else lastStatement->nextStatement = other;
        lastStatement = lastOther;
    }

    root->statement = firstStatement;
}





// First and last can be the same.
void AddStatements(HLSLRoot * root, HLSLStatement * before, HLSLStatement * first, HLSLStatement * last)
{
    if (before == NULL) {
        last->nextStatement = root->statement;
        root->statement = first;
    }
    else {
        last->nextStatement = before->nextStatement;
        before->nextStatement = first;
    }
}

void AddSingleStatement(HLSLRoot * root, HLSLStatement * before, HLSLStatement * statement)
{
    AddStatements(root, before, statement, statement);
}



// @@ This is very game-specific. Should be moved to pipeline_parser or somewhere else.
void GroupParameters(HLSLTree * tree)
{
    // Sort parameters based on semantic and group them in cbuffers.

    HLSLRoot* root = tree->GetRoot();

    HLSLDeclaration * firstPerItemDeclaration = NULL;
    HLSLDeclaration * lastPerItemDeclaration = NULL;

    HLSLDeclaration * instanceDataDeclaration = NULL;

    HLSLDeclaration * firstPerPassDeclaration = NULL;
    HLSLDeclaration * lastPerPassDeclaration = NULL;

    HLSLDeclaration * firstPerItemSampler = NULL;
    HLSLDeclaration * lastPerItemSampler = NULL;

    HLSLDeclaration * firstPerPassSampler = NULL;
    HLSLDeclaration * lastPerPassSampler = NULL;

    HLSLStatement * statementBeforeBuffers = NULL;
    
    HLSLStatement* previousStatement = NULL;
    HLSLStatement* statement = root->statement;
    while (statement != NULL)
    {
        HLSLStatement* nextStatement = statement->nextStatement;

        if (statement->nodeType == HLSLNodeType::Struct) // Do not remove this, or it will mess the else clause below.
        {   
            statementBeforeBuffers = statement;
        }
        else if (statement->nodeType == HLSLNodeType::Declaration)
        {
            HLSLDeclaration* declaration = (HLSLDeclaration*)statement;

            // We insert buffers after the last const declaration.
            if ((declaration->type.flags & (int)HLSLTypeFlags::Const) != 0)
            {
                statementBeforeBuffers = statement;
            }

            // Do not move samplers or static/const parameters.
            if ((declaration->type.flags & ((int)HLSLTypeFlags::Static | (int)HLSLTypeFlags::Const)) == 0)
            {
                // Unlink statement.
                statement->nextStatement = NULL;
                if (previousStatement != NULL) previousStatement->nextStatement = nextStatement;
                else root->statement = nextStatement;

                while(declaration != NULL)
                {
                    HLSLDeclaration* nextDeclaration = declaration->nextDeclaration;

                    if (declaration->semantic != NULL && String_EqualNoCase(declaration->semantic, "PER_INSTANCED_ITEM"))
                    {
                        ASSERT(instanceDataDeclaration == NULL);
                        instanceDataDeclaration = declaration;
                    }
                    else
                    {
                        // Select group based on type and semantic.
                        HLSLDeclaration ** first, ** last;
                        if (declaration->semantic == NULL || String_EqualNoCase(declaration->semantic, "PER_ITEM") || String_EqualNoCase(declaration->semantic, "PER_MATERIAL"))
                        {
                            if (IsSamplerType(declaration->type))
                            {
                                first = &firstPerItemSampler;
                                last = &lastPerItemSampler;
                            }
                            else
                            {
                                first = &firstPerItemDeclaration;
                                last = &lastPerItemDeclaration;
                            }
                        }
                        else
                        {
                            if (IsSamplerType(declaration->type))
                            {
                                first = &firstPerPassSampler;
                                last = &lastPerPassSampler;
                            }
                            else
                            {
                                first = &firstPerPassDeclaration;
                                last = &lastPerPassDeclaration;
                            }
                        }

                        // Add declaration to new list.
                        if (*first == NULL) *first = declaration;
                        else (*last)->nextStatement = declaration;
                        *last = declaration;
                    }

                    // Unlink from declaration list.
                    declaration->nextDeclaration = NULL;

                    // Reset attributes.
                    declaration->registerName = NULL;
                    //declaration->semantic = NULL;         // @@ Don't do this!

                    declaration = nextDeclaration;
                }
            }
        }
        /*else
        {
            if (statementBeforeBuffers == NULL) {
                // This is the location where we will insert our buffers.
                statementBeforeBuffers = previousStatement;
            }
        }*/

        if (statement->nextStatement == nextStatement) {
            previousStatement = statement;
        }
        statement = nextStatement;
    }


    // Add instance data declaration at the end of the per_item buffer.
    if (instanceDataDeclaration != NULL)
    {
        if (firstPerItemDeclaration == NULL) firstPerItemDeclaration = instanceDataDeclaration;
        else lastPerItemDeclaration->nextStatement = instanceDataDeclaration;
    }


    // Add samplers.
    if (firstPerItemSampler != NULL) {
        AddStatements(root, statementBeforeBuffers, firstPerItemSampler, lastPerItemSampler);
        statementBeforeBuffers = lastPerItemSampler;
    }
    if (firstPerPassSampler != NULL) {
        AddStatements(root, statementBeforeBuffers, firstPerPassSampler, lastPerPassSampler);
        statementBeforeBuffers = lastPerPassSampler;
    }


    // @@ We are assuming per_item and per_pass buffers don't already exist. @@ We should assert on that.

    if (firstPerItemDeclaration != NULL)
    {
        // Create buffer statement.
        HLSLBuffer * perItemBuffer = tree->AddNode<HLSLBuffer>(firstPerItemDeclaration->fileName, firstPerItemDeclaration->line-1);
        perItemBuffer->name = tree->AddString("per_item");
        perItemBuffer->registerName = tree->AddString("b0");
        perItemBuffer->field = firstPerItemDeclaration;
        
        // Set declaration buffer pointers.
        HLSLDeclaration * field = perItemBuffer->field;
        while (field != NULL)
        {
            field->buffer = perItemBuffer;
            field = (HLSLDeclaration *)field->nextStatement;
        }

        // Add buffer to statements.
        AddSingleStatement(root, statementBeforeBuffers, perItemBuffer);
        statementBeforeBuffers = perItemBuffer;
    }

    if (firstPerPassDeclaration != NULL)
    {
        // Create buffer statement.
        HLSLBuffer * perPassBuffer = tree->AddNode<HLSLBuffer>(firstPerPassDeclaration->fileName, firstPerPassDeclaration->line-1);
        perPassBuffer->name = tree->AddString("per_pass");
        perPassBuffer->registerName = tree->AddString("b1");
        perPassBuffer->field = firstPerPassDeclaration;

        // Set declaration buffer pointers.
        HLSLDeclaration * field = perPassBuffer->field;
        while (field != NULL)
        {
            field->buffer = perPassBuffer;
            field = (HLSLDeclaration *)field->nextStatement;
        }
        
        // Add buffer to statements.
        AddSingleStatement(root, statementBeforeBuffers, perPassBuffer);
    }
}


class FindArgumentVisitor : public HLSLTreeVisitor
{
public:
    bool found;
    const char * name;

	FindArgumentVisitor()
	{
		found = false;
		name  = NULL;
	}

    bool FindArgument(const char * name, HLSLFunction * function)
    {
        this->found = false;
        this->name = name;
        VisitStatements(function->statement);
        return found;
    }
    
    virtual void VisitStatements(HLSLStatement * statement) override
    {
        while (statement != NULL && !found)
        {
            VisitStatement(statement);
            statement = statement->nextStatement;
        }
    }

    virtual void VisitIdentifierExpression(HLSLIdentifierExpression * node) override
    {
        if (node->name == name)
        {
            found = true;
        }
    }
};


void HideUnusedArguments(HLSLFunction * function)
{
    FindArgumentVisitor visitor;
 
    // For each argument.
    HLSLArgument * arg = function->argument;
    while (arg != NULL)
    {
        if (!visitor.FindArgument(arg->name, function))
        {
            arg->hidden = true;
        }

        arg = arg->nextArgument;
    }
}

bool EmulateAlphaTest(HLSLTree* tree, const char* entryName, float alphaRef/*=0.5*/)
{
    // Find all return statements of this entry point.
    HLSLFunction* entry = tree->FindFunction(entryName);
    if (entry != NULL)
    {
        HLSLStatement ** ptr = &entry->statement;
        HLSLStatement * statement = entry->statement;
        while (statement != NULL)
        {
            if (statement->nodeType == HLSLNodeType::ReturnStatement)
            {
                HLSLReturnStatement * returnStatement = (HLSLReturnStatement *)statement;
                HLSLBaseType returnType = returnStatement->expression->expressionType.baseType;
                
                // Build statement: "if (%s.a < 0.5) discard;"

                HLSLDiscardStatement * discard = tree->AddNode<HLSLDiscardStatement>(statement->fileName, statement->line);
                
                HLSLExpression * alpha = NULL;
                if (returnType == HLSLBaseType::Float4 || returnType == HLSLBaseType::Half4)
                {
                    // @@ If return expression is a constructor, grab 4th argument.
                    // That's not as easy, since we support 'float4(float3, float)' or 'float4(float, float3)', extracting
                    // the latter is not that easy.
                    /*if (returnStatement->expression->nodeType == HLSLNodeType::ConstructorExpression) {
                        HLSLConstructorExpression * constructor = (HLSLConstructorExpression *)returnStatement->expression;
                        //constructor->
                    }
                    */
                    
                    if (alpha == NULL) {
                        HLSLMemberAccess * access = tree->AddNode<HLSLMemberAccess>(statement->fileName, statement->line);
                        access->expressionType = HLSLType(HLSLBaseType::Float);
                        access->object = returnStatement->expression;     // @@ Is reference OK? Or should we clone expression?
                        access->field = tree->AddString("a");
                        access->swizzle = true;
                        
                        alpha = access;
                    }
                }
                else if (returnType == HLSLBaseType::Float || returnType == HLSLBaseType::Half)
                {
                    alpha = returnStatement->expression;     // @@ Is reference OK? Or should we clone expression?
                }
                else
                {
                    return false;
                }
                
                HLSLLiteralExpression * threshold = tree->AddNode<HLSLLiteralExpression>(statement->fileName, statement->line);
                threshold->expressionType = HLSLType(HLSLBaseType::Float);
                threshold->fValue = alphaRef;
                threshold->type = HLSLBaseType::Float;
                
                HLSLBinaryExpression * condition = tree->AddNode<HLSLBinaryExpression>(statement->fileName, statement->line);
                condition->expressionType = HLSLType(HLSLBaseType::Bool);
                condition->binaryOp = HLSLBinaryOp::Less;
                condition->expression1 = alpha;
                condition->expression2 = threshold;

                // Insert statement.
                HLSLIfStatement * st = tree->AddNode<HLSLIfStatement>(statement->fileName, statement->line);
                st->condition = condition;
                st->statement = discard;
                st->nextStatement = statement;
                *ptr = st;
            }
        
            ptr = &statement->nextStatement;
            statement = statement->nextStatement;
        }
    }

    return true;
}

bool NeedsFlattening(HLSLExpression * expr, int level = 0) {
    if (expr == NULL) {
        return false;
    }
    if (expr->nodeType == HLSLNodeType::UnaryExpression) {
        HLSLUnaryExpression * unaryExpr = (HLSLUnaryExpression *)expr;
        return NeedsFlattening(unaryExpr->expression, level+1) || NeedsFlattening(expr->nextExpression, level);
    }
    else if (expr->nodeType == HLSLNodeType::BinaryExpression) {
        HLSLBinaryExpression * binaryExpr = (HLSLBinaryExpression *)expr;
        if (IsAssignOp(binaryExpr->binaryOp)) {
            return NeedsFlattening(binaryExpr->expression2, level+1) || NeedsFlattening(expr->nextExpression, level);
        }
        else {
            return NeedsFlattening(binaryExpr->expression1, level+1) || NeedsFlattening(binaryExpr->expression2, level+1) || NeedsFlattening(expr->nextExpression, level);
        }
    }
    else if (expr->nodeType == HLSLNodeType::ConditionalExpression) {
        HLSLConditionalExpression * conditionalExpr = (HLSLConditionalExpression *)expr;
        return NeedsFlattening(conditionalExpr->condition, level+1) || NeedsFlattening(conditionalExpr->trueExpression, level+1) || NeedsFlattening(conditionalExpr->falseExpression, level+1) || NeedsFlattening(expr->nextExpression, level);
    }
    else if (expr->nodeType == HLSLNodeType::CastingExpression) {
        HLSLCastingExpression * castingExpr = (HLSLCastingExpression *)expr;
        return NeedsFlattening(castingExpr->expression, level+1) || NeedsFlattening(expr->nextExpression, level);
    }
    else if (expr->nodeType == HLSLNodeType::LiteralExpression) {
        return NeedsFlattening(expr->nextExpression, level);
    }
    else if (expr->nodeType == HLSLNodeType::IdentifierExpression) {
        return NeedsFlattening(expr->nextExpression, level);
    }
    else if (expr->nodeType == HLSLNodeType::ConstructorExpression) {
        HLSLConstructorExpression * constructorExpr = (HLSLConstructorExpression *)expr;
        return NeedsFlattening(constructorExpr->argument, level+1) || NeedsFlattening(expr->nextExpression, level);
    }
    else if (expr->nodeType == HLSLNodeType::MemberAccess) {
        return NeedsFlattening(expr->nextExpression, level+1);
    }
    else if (expr->nodeType == HLSLNodeType::ArrayAccess) {
        HLSLArrayAccess * arrayAccess = (HLSLArrayAccess *)expr;
        return NeedsFlattening(arrayAccess->array, level+1) || NeedsFlattening(arrayAccess->index, level+1) || NeedsFlattening(expr->nextExpression, level);
    }
    else if (expr->nodeType == HLSLNodeType::FunctionCall) {
        HLSLFunctionCall * functionCall = (HLSLFunctionCall *)expr;
        if (functionCall->function->numOutputArguments > 0) {
            if (level > 0) {
                return true;
            }
        }
        return NeedsFlattening(functionCall->argument, level+1) || NeedsFlattening(expr->nextExpression, level);
    }
    else {
        //assert(false);
        return false;
    }
}


struct StatementList {
    HLSLStatement * head = NULL;
    HLSLStatement * tail = NULL;
    void append(HLSLStatement * st) {
        if (head == NULL) {
            tail = head = st;
        }
        tail->nextStatement = st;
        tail = st;
    }
};


    class ExpressionFlattener : public HLSLTreeVisitor
    {
    public:
        HLSLTree * m_tree;
        int tmp_index;
        HLSLStatement ** statement_pointer;
        HLSLFunction * current_function;
        
        ExpressionFlattener()
        {
            m_tree = NULL;
            tmp_index = 0;
            statement_pointer = NULL;
            current_function = NULL;
        }
        
        void FlattenExpressions(HLSLTree * tree)
        {
            m_tree = tree;
            VisitRoot(tree->GetRoot());
        }

        // Visit all statements updating the statement_pointer so that we can insert and replace statements. @@ Add this to the default visitor?
        virtual void VisitFunction(HLSLFunction * node) override
        {
            current_function = node;
            statement_pointer = &node->statement;
            VisitStatements(node->statement);
            statement_pointer = NULL;
            current_function = NULL;
        }

        virtual void VisitIfStatement(HLSLIfStatement * node) override
        {
            if (NeedsFlattening(node->condition, 1)) {
                assert(false);  // @@ Add statements before if statement.
            }
            
            statement_pointer = &node->statement;
            VisitStatements(node->statement);
            if (node->elseStatement) {
                statement_pointer = &node->elseStatement;
                VisitStatements(node->elseStatement);
            }
        }
        
        virtual void VisitForStatement(HLSLForStatement * node) override
        {
            if (NeedsFlattening(node->initialization->assignment, 1)) {
                assert(false);  // @@ Add statements before for statement.
            }
            if (NeedsFlattening(node->condition, 1) || NeedsFlattening(node->increment, 1)) {
                assert(false);  // @@ These are tricky to implement. Need to handle all loop exits.
            }

            statement_pointer = &node->statement;
            VisitStatements(node->statement);
        }
        
        virtual void VisitBlockStatement(HLSLBlockStatement * node) override
        {
            statement_pointer = &node->statement;
            VisitStatements(node->statement);
        }
        
        virtual void VisitStatements(HLSLStatement * statement) override
        {
            while (statement != NULL) {
                VisitStatement(statement);
                statement_pointer = &statement->nextStatement;
                statement = statement->nextStatement;
            }
        }

        // This is usually a function call or assignment.
        virtual void VisitExpressionStatement(HLSLExpressionStatement * node) override
        {
            if (NeedsFlattening(node->expression, 0))
            {
                StatementList statements;
                Flatten(node->expression, statements, false);
                
                // Link beginning of statement list.
                *statement_pointer = statements.head;

                // Link end of statement list.
                HLSLStatement * tail = statements.tail;
                tail->nextStatement = node->nextStatement;
                
                // Update statement pointer.
                statement_pointer = &tail->nextStatement;
                
                // @@ Delete node?
            }
        }

        virtual void VisitDeclaration(HLSLDeclaration * node) override
        {
            // Skip global declarations.
            if (statement_pointer == NULL) return;
            
            if (NeedsFlattening(node->assignment, 1))
            {
                StatementList statements;
                HLSLIdentifierExpression * ident = Flatten(node->assignment, statements, true);
                
                // @@ Delete node->assignment?
                
                node->assignment = ident;
                statements.append(node);
                
                // Link beginning of statement list.
                *statement_pointer = statements.head;
                
                // Link end of statement list.
                HLSLStatement * tail = statements.tail;
                tail->nextStatement = node->nextStatement;
                
                // Update statement pointer.
                statement_pointer = &tail->nextStatement;
            }
        }

        virtual void VisitReturnStatement(HLSLReturnStatement * node) override
        {
            if (NeedsFlattening(node->expression, 1))
            {
                StatementList statements;
                HLSLIdentifierExpression * ident = Flatten(node->expression, statements, true);

                // @@ Delete node->expression?
                
                node->expression = ident;
                statements.append(node);
                
                // Link beginning of statement list.
                *statement_pointer = statements.head;
                
                // Link end of statement list.
                HLSLStatement * tail = statements.tail;
                tail->nextStatement = node->nextStatement;
                
                // Update statement pointer.
                statement_pointer = &tail->nextStatement;
            }
        }

        
        HLSLDeclaration * BuildTemporaryDeclaration(HLSLExpression * expr)
        {
            assert(expr->expressionType.baseType != HLSLBaseType::Void);
            
            HLSLDeclaration * declaration = m_tree->AddNode<HLSLDeclaration>(expr->fileName, expr->line);
            declaration->name = m_tree->AddStringFormat("tmp%d", tmp_index++);
            declaration->type = expr->expressionType;
            declaration->assignment = expr;
            
            HLSLIdentifierExpression * ident = (HLSLIdentifierExpression *)expr;
            
            return declaration;
        }

        HLSLExpressionStatement * BuildExpressionStatement(HLSLExpression * expr)
        {
            HLSLExpressionStatement * statement = m_tree->AddNode<HLSLExpressionStatement>(expr->fileName, expr->line);
            statement->expression = expr;
            return statement;
        }

        HLSLIdentifierExpression * AddExpressionStatement(HLSLExpression * expr, StatementList & statements, bool wantIdent)
        {
            if (wantIdent) {
                HLSLDeclaration * declaration = BuildTemporaryDeclaration(expr);
                statements.append(declaration);
                
                HLSLIdentifierExpression * ident = m_tree->AddNode<HLSLIdentifierExpression>(expr->fileName, expr->line);
                ident->name = declaration->name;
                ident->expressionType = declaration->type;
                return ident;
            }
            else {
                HLSLExpressionStatement * statement = BuildExpressionStatement(expr);
                statements.append(statement);
                return NULL;
            }
        }
        
        HLSLIdentifierExpression * Flatten(HLSLExpression * expr, StatementList & statements, bool wantIdent = true)
        {
            if (!NeedsFlattening(expr, wantIdent)) {
                return AddExpressionStatement(expr, statements, wantIdent);
            }
            
            if (expr->nodeType == HLSLNodeType::UnaryExpression) {
                assert(expr->nextExpression == NULL);
                
                HLSLUnaryExpression * unaryExpr = (HLSLUnaryExpression *)expr;
                
                HLSLIdentifierExpression * tmp = Flatten(unaryExpr->expression, statements, true);
                
                HLSLUnaryExpression * newUnaryExpr = m_tree->AddNode<HLSLUnaryExpression>(unaryExpr->fileName, unaryExpr->line);
                newUnaryExpr->unaryOp = unaryExpr->unaryOp;
                newUnaryExpr->expression = tmp;
                newUnaryExpr->expressionType = unaryExpr->expressionType;

                return AddExpressionStatement(newUnaryExpr, statements, wantIdent);
            }
            else if (expr->nodeType == HLSLNodeType::BinaryExpression) {
                assert(expr->nextExpression == NULL);
                
                HLSLBinaryExpression * binaryExpr = (HLSLBinaryExpression *)expr;
                
                if (IsAssignOp(binaryExpr->binaryOp)) {
                    // Flatten right hand side only.
                    HLSLIdentifierExpression * tmp2 = Flatten(binaryExpr->expression2, statements, true);
                    
                    HLSLBinaryExpression * newBinaryExpr = m_tree->AddNode<HLSLBinaryExpression>(binaryExpr->fileName, binaryExpr->line);
                    newBinaryExpr->binaryOp = binaryExpr->binaryOp;
                    newBinaryExpr->expression1 = binaryExpr->expression1;
                    newBinaryExpr->expression2 = tmp2;
                    newBinaryExpr->expressionType = binaryExpr->expressionType;
                    
                    return AddExpressionStatement(newBinaryExpr, statements, wantIdent);
                }
                else {
                    HLSLIdentifierExpression * tmp1 = Flatten(binaryExpr->expression1, statements, true);
                    HLSLIdentifierExpression * tmp2 = Flatten(binaryExpr->expression2, statements, true);

                    HLSLBinaryExpression * newBinaryExpr = m_tree->AddNode<HLSLBinaryExpression>(binaryExpr->fileName, binaryExpr->line);
                    newBinaryExpr->binaryOp = binaryExpr->binaryOp;
                    newBinaryExpr->expression1 = tmp1;
                    newBinaryExpr->expression2 = tmp2;
                    newBinaryExpr->expressionType = binaryExpr->expressionType;
                    
                    return AddExpressionStatement(newBinaryExpr, statements, wantIdent);
                }
            }
            else if (expr->nodeType == HLSLNodeType::ConditionalExpression) {
                assert(false);
            }
            else if (expr->nodeType == HLSLNodeType::CastingExpression) {
                assert(false);
            }
            else if (expr->nodeType == HLSLNodeType::LiteralExpression) {
                assert(false);
            }
            else if (expr->nodeType == HLSLNodeType::IdentifierExpression) {
                assert(false);
            }
            else if (expr->nodeType == HLSLNodeType::ConstructorExpression) {
                assert(false);
            }
            else if (expr->nodeType == HLSLNodeType::MemberAccess) {
                assert(false);
            }
            else if (expr->nodeType == HLSLNodeType::ArrayAccess) {
                assert(false);
            }
            else if (expr->nodeType == HLSLNodeType::FunctionCall) {
                HLSLFunctionCall * functionCall = (HLSLFunctionCall *)expr;

                // @@ Output function as is?
                // @@ We have to flatten function arguments! This is tricky, need to handle input/output arguments.
                assert(!NeedsFlattening(functionCall->argument));
                
                return AddExpressionStatement(expr, statements, wantIdent);
            }
            else {
                assert(false);
            }
            return NULL;
        }
    };

    
void FlattenExpressions(HLSLTree* tree) {
    ExpressionFlattener flattener;
    flattener.FlattenExpressions(tree);
}

} // M4

