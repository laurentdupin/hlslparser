#ifndef HLSL_TREE_H
#define HLSL_TREE_H

//#include "Engine/StringPool.h"
#include "Engine.h"

#include <new>

namespace M4
{

enum class HLSLNodeType
{
    Root,
    Declaration,
    Struct,
    StructField,
    Buffer,
    BufferField,
    Function,
    Argument,
    ExpressionStatement,
    Expression,
    ReturnStatement,
    DiscardStatement,
    BreakStatement,
    ContinueStatement,
    IfStatement,
    ForStatement,
    BlockStatement,
    UnaryExpression,
    BinaryExpression,
    ConditionalExpression,
    CastingExpression,
    LiteralExpression,
    IdentifierExpression,
    ConstructorExpression,
    MemberAccess,
    ArrayAccess,
    FunctionCall,
    StateAssignment,
    SamplerState,
    Pass,
    Technique,
    Attribute,
    Pipeline,
    Stage,
};

enum class HLSLTypeDimension
{
    None,
    Scalar,
    Vector2,
    Vector3,
    Vector4,
    Matrix2x2,
    Matrix3x3,
    Matrix4x4,
    Matrix4x3,
    Matrix4x2
};
    
enum class HLSLBaseType
{
    Unknown,
    Void,    
    Float,
    FirstNumeric = HLSLBaseType::Float,
    Float2,
    Float3,
    Float4,
	Float2x2,
    Float3x3,
    Float4x4,
    Float4x3,
    Float4x2,
    Half,
    Half2,
    Half3,
    Half4,
	Half2x2,
    Half3x3,
    Half4x4,
    Half4x3,
    Half4x2,
    Bool,
    FirstInteger = HLSLBaseType::Bool,
	Bool2,
	Bool3,
	Bool4,
    Int,
    Int2,
    Int3,
    Int4,
    Uint,
    Uint2,
    Uint3,
    Uint4,
  /*Short,   // @@ Separate dimension from Base type, this is getting out of control.
    Short2,
    Short3,
    Short4,
    Ushort,
    Ushort2,
    Ushort3,
    Ushort4,*/
    LastInteger = HLSLBaseType::Uint4,
    LastNumeric = HLSLBaseType::Uint4,
    Texture,
    Sampler,           // @@ use type inference to determine sampler type.
    Sampler2D,
    Sampler3D,
    SamplerCube,
    Sampler2DShadow,
    Sampler2DMS,
    Sampler2DArray,
    Texture1D,
    Texture1DArray,
    Texture2D,
    Texture2DArray,
    Texture2DMS,
    Texture2DMSArray,
    Texture3D,
    TextureCube,
    TextureCubeArray,
    SamplerState,
    UserDefined,       // struct
    Expression,        // type argument for defined() sizeof() and typeof().
    Auto,
    
    Count,
    NumericCount = LastNumeric - FirstNumeric + 1
};
    
extern const HLSLTypeDimension BaseTypeDimension[(int)HLSLBaseType::Count];
extern const HLSLBaseType ScalarBaseType[(int)HLSLBaseType::Count];

inline bool IsSamplerType(HLSLBaseType baseType)
{
    return baseType == HLSLBaseType::Sampler ||
           baseType == HLSLBaseType::Sampler2D ||
           baseType == HLSLBaseType::Sampler3D ||
           baseType == HLSLBaseType::SamplerCube ||
           baseType == HLSLBaseType::Sampler2DShadow ||
           baseType == HLSLBaseType::Sampler2DMS ||
           baseType == HLSLBaseType::Sampler2DArray;
}

inline bool IsTextureType(HLSLBaseType baseType)
{
    return baseType == HLSLBaseType::Texture1D ||
           baseType == HLSLBaseType::Texture1DArray ||
           baseType == HLSLBaseType::Texture2D ||
           baseType == HLSLBaseType::Texture2DArray ||
           baseType == HLSLBaseType::Texture2DMS ||
           baseType == HLSLBaseType::Texture2DMSArray ||
           baseType == HLSLBaseType::Texture3D ||
           baseType == HLSLBaseType::TextureCube ||
           baseType == HLSLBaseType::TextureCubeArray;
}

inline bool IsMatrixType(HLSLBaseType baseType)
{
    return baseType == HLSLBaseType::Float3x3 || baseType == HLSLBaseType::Float4x4 || baseType == HLSLBaseType::Float4x3 || baseType == HLSLBaseType::Float4x2 ||
        baseType == HLSLBaseType::Half3x3 || baseType == HLSLBaseType::Half4x4 || baseType == HLSLBaseType::Half4x3 || baseType == HLSLBaseType::Half4x2;
}

inline bool IsScalarType( HLSLBaseType baseType )
{
	return  baseType == HLSLBaseType::Float ||
			baseType == HLSLBaseType::Half ||
			baseType == HLSLBaseType::Bool ||
			baseType == HLSLBaseType::Int ||
			baseType == HLSLBaseType::Uint;
}

inline bool IsVectorType( HLSLBaseType baseType )
{
	return  baseType == HLSLBaseType::Float2 ||
		baseType == HLSLBaseType::Float3 ||
		baseType == HLSLBaseType::Float4 ||
		baseType == HLSLBaseType::Half2 ||
		baseType == HLSLBaseType::Half3 ||
		baseType == HLSLBaseType::Half4 ||
		baseType == HLSLBaseType::Bool2 ||
		baseType == HLSLBaseType::Bool3 ||
		baseType == HLSLBaseType::Bool4 ||
		baseType == HLSLBaseType::Int2  ||
		baseType == HLSLBaseType::Int3  ||
		baseType == HLSLBaseType::Int4  ||
		baseType == HLSLBaseType::Uint2 ||
		baseType == HLSLBaseType::Uint3 ||
		baseType == HLSLBaseType::Uint4;
}


enum class HLSLBinaryOp
{
    And,
    Or,
    Add,
    Sub,
    Mul,
    Div,
    Less,
    Greater,
    LessEqual,
    GreaterEqual,
    Equal,
    NotEqual,
    BitAnd,
    BitOr,
    BitXor,
    Assign,
    AddAssign,
    SubAssign,
    MulAssign,
    DivAssign,
};

inline bool IsCompareOp( HLSLBinaryOp op )
{
	return op == HLSLBinaryOp::Less ||
		op == HLSLBinaryOp::Greater ||
		op == HLSLBinaryOp::LessEqual ||
		op == HLSLBinaryOp::GreaterEqual ||
		op == HLSLBinaryOp::Equal ||
		op == HLSLBinaryOp::NotEqual;
}

inline bool IsArithmeticOp( HLSLBinaryOp op )
{
    return op == HLSLBinaryOp::Add ||
        op == HLSLBinaryOp::Sub ||
        op == HLSLBinaryOp::Mul ||
        op == HLSLBinaryOp::Div;
}

inline bool IsLogicOp( HLSLBinaryOp op )
{
    return op == HLSLBinaryOp::And ||
        op == HLSLBinaryOp::Or;
}

inline bool IsAssignOp( HLSLBinaryOp op )
{
    return op == HLSLBinaryOp::Assign ||
        op == HLSLBinaryOp::AddAssign ||
        op == HLSLBinaryOp::SubAssign ||
        op == HLSLBinaryOp::MulAssign ||
        op == HLSLBinaryOp::DivAssign;
}

    
enum class HLSLUnaryOp
{
    Negative,       // -x
    Positive,       // +x
    Not,            // !x
    PreIncrement,   // ++x
    PreDecrement,   // --x
    PostIncrement,  // x++
    PostDecrement,  // x++
    BitNot,         // ~x
};

enum class HLSLArgumentModifier
{
    None,
    In,
    Out,
    Inout,
    Uniform,
    Const,
};

enum class HLSLTypeFlags
{
    None = 0,
    Const = 0x01,
    Static = 0x02, 
    Input = 0x04,
    Output = 0x08,
    Linear = 0x10,
    Centroid = 0x20,

    // Interpolation modifiers.
    NoInterpolation = 0x40,
    NoPerspective = 0x80,
    Sample = 0x100,

    // Misc.
    NoPromote = 0x200,

    //Uniform = 0x400,
    //Extern = 0x800,
    //Volatile = 0x1000,
    //Shared = 0x2000,
    //Precise = 0x4000,
};

enum class HLSLAttributeType
{
    Unknown,
    Unroll,
    Branch,
    Flatten,
    NoFastMath,
};

enum class HLSLAddressSpace
{
    Undefined,
    Constant,
    Device,
    Thread,
    Shared,
};


struct HLSLNode;
struct HLSLRoot;
struct HLSLStatement;
struct HLSLAttribute;
struct HLSLDeclaration;
struct HLSLStruct;
struct HLSLStructField;
struct HLSLBuffer;
struct HLSLFunction;
struct HLSLArgument;
struct HLSLExpressionStatement;
struct HLSLExpression;
struct HLSLBinaryExpression;
struct HLSLLiteralExpression;
struct HLSLIdentifierExpression;
struct HLSLConstructorExpression;
struct HLSLFunctionCall;
struct HLSLArrayAccess;
struct HLSLAttribute;

struct HLSLType
{
    explicit HLSLType(HLSLBaseType _baseType = HLSLBaseType::Unknown)
    { 
        baseType    = _baseType;
        samplerType = HLSLBaseType::Float;
        textureType = HLSLBaseType::Float4;
        typeName    = NULL;
        array       = false;
        arraySize   = NULL;
        flags       = 0;
        addressSpace = HLSLAddressSpace::Undefined;
    }
    HLSLBaseType        baseType;
    HLSLBaseType        samplerType;    // Half or Float
    HLSLBaseType        textureType;    // Half or Float
    const char*         typeName;       // For user defined types.
    bool                array;
    HLSLExpression*     arraySize;
    int                 flags;
    HLSLAddressSpace    addressSpace;

    virtual nlohmann::json      ConvertToJSON();
};

inline bool IsTextureType(const HLSLType& type)
{
    return IsTextureType(type.baseType);
}

inline bool IsSamplerType(const HLSLType & type)
{
    return IsSamplerType(type.baseType);
}

inline bool IsScalarType(const HLSLType & type)
{
	return IsScalarType(type.baseType);
}

inline bool IsVectorType(const HLSLType & type)
{
	return IsVectorType(type.baseType);
}


/** Base class for all nodes in the HLSL AST */
struct HLSLNode
{
    HLSLNodeType                nodeType;
    const char*                 fileName = NULL;
    int                         line = 0;

    virtual nlohmann::json      ConvertToJSON(bool bNodeType = true);
};

struct HLSLRoot : public HLSLNode
{
    static const HLSLNodeType s_type = HLSLNodeType::Root;
    HLSLRoot()          { statement = NULL; }
    HLSLStatement*      statement;          // First statement.

    virtual nlohmann::json      ConvertToJSON(bool bNodeType = true) override;
};

struct HLSLStatement : public HLSLNode
{
    HLSLStatement() 
    { 
        nextStatement   = NULL; 
        attributes      = NULL;
        hidden          = false;
    }
    HLSLStatement*      nextStatement;      // Next statement in the block.
    HLSLAttribute*      attributes;
    mutable bool        hidden;

    virtual nlohmann::json      ConvertToJSON(bool bNodeType = true) override;
};

struct HLSLAttribute : public HLSLNode
{
    static const HLSLNodeType s_type = HLSLNodeType::Attribute;
	HLSLAttribute()
	{
		attributeType = HLSLAttributeType::Unknown;
		argument      = NULL;
		nextAttribute = NULL;
	}
    HLSLAttributeType   attributeType;
    HLSLExpression*     argument;
    HLSLAttribute*      nextAttribute;

    virtual nlohmann::json      ConvertToJSON(bool bNodeType = true) override;
};

struct HLSLDeclaration : public HLSLStatement
{
    static const HLSLNodeType s_type = HLSLNodeType::Declaration;
    HLSLDeclaration()
    {
        name            = NULL;
        registerName    = NULL;
        spaceName       = NULL;
        semantic        = NULL;
        nextDeclaration = NULL;
        assignment      = NULL;
        buffer          = NULL;
    }
    const char*         name;
    HLSLType            type;
    const char*         registerName;       // @@ Store register index?
    const char*         spaceName;       // @@ Store register index?
    const char*         semantic;
    HLSLDeclaration*    nextDeclaration;    // If multiple variables declared on a line.
    HLSLExpression*     assignment;
    HLSLBuffer*         buffer;

    virtual nlohmann::json      ConvertToJSON(bool bNodeType = true) override;
};

struct HLSLStruct : public HLSLStatement
{
    static const HLSLNodeType s_type = HLSLNodeType::Struct;
    HLSLStruct()
    {
        name            = NULL;
        field           = NULL;
    }
    const char*         name;
    HLSLStructField*    field;              // First field in the structure.

    virtual nlohmann::json      ConvertToJSON(bool bNodeType = true) override;
};

struct HLSLStructField : public HLSLNode
{
    static const HLSLNodeType s_type = HLSLNodeType::StructField;
    HLSLStructField()
    {
        name            = NULL;
        semantic        = NULL;
        sv_semantic     = NULL;
        nextField       = NULL;
        hidden          = false;
    }
    const char*         name;
    HLSLType            type;
    const char*         semantic;
    const char*         sv_semantic;
    HLSLStructField*    nextField;      // Next field in the structure.
    bool                hidden;

    virtual nlohmann::json      ConvertToJSON(bool bNodeType = true) override;

};

/** A cbuffer or tbuffer declaration. */
struct HLSLBuffer : public HLSLStatement
{
    static const HLSLNodeType s_type = HLSLNodeType::Buffer;
    HLSLBuffer()
    {
        name            = NULL;
        registerName    = NULL;
        spaceName       = NULL;
        field           = NULL;
    }
    const char*         name;
    const char*         registerName;
    const char*         spaceName;
    HLSLDeclaration*    field;

    virtual nlohmann::json      ConvertToJSON(bool bNodeType = true) override;
};


/** Function declaration */
struct HLSLFunction : public HLSLStatement
{
    static const HLSLNodeType s_type = HLSLNodeType::Function;
    HLSLFunction()
    {
        name            = NULL;
        semantic        = NULL;
        sv_semantic     = NULL;
        statement       = NULL;
        argument        = NULL;
        numArguments    = 0;
        numOutputArguments = 0;
        forward         = NULL;
    }
    const char*         name;
    HLSLType            returnType;
    HLSLBaseType        memberOfType = HLSLBaseType::Void;
    const char*         semantic;
    const char*         sv_semantic;
    int                 numArguments;
    int                 numOutputArguments;     // Includes out and inout arguments.
    HLSLArgument*       argument;
    HLSLStatement*      statement;
    HLSLFunction*       forward; // Which HLSLFunction this one forward-declares

    virtual nlohmann::json      ConvertToJSON(bool bNodeType = true) override;
};

/** Declaration of an argument to a function. */
struct HLSLArgument : public HLSLNode
{
    static const HLSLNodeType s_type = HLSLNodeType::Argument;
    HLSLArgument()
    {
        name            = NULL;
        modifier        = HLSLArgumentModifier::None;
        semantic        = NULL;
        sv_semantic     = NULL;
        defaultValue    = NULL;
        nextArgument    = NULL;
        hidden          = false;
    }
    const char*             name;
    HLSLArgumentModifier    modifier;
    HLSLType                type;
    const char*             semantic;
    const char*             sv_semantic;
    HLSLExpression*         defaultValue;
    HLSLArgument*           nextArgument;
    bool                    hidden;

    virtual nlohmann::json      ConvertToJSON(bool bNodeType = true) override;
};

/** A expression which forms a complete statement. */
struct HLSLExpressionStatement : public HLSLStatement
{
    static const HLSLNodeType s_type = HLSLNodeType::ExpressionStatement;
    HLSLExpressionStatement()
    {
        expression = NULL;
    }
    HLSLExpression*     expression;
};

struct HLSLReturnStatement : public HLSLStatement
{
    static const HLSLNodeType s_type = HLSLNodeType::ReturnStatement;
    HLSLReturnStatement()
    {
        expression = NULL;
    }
    HLSLExpression*     expression;
};

struct HLSLDiscardStatement : public HLSLStatement
{
    static const HLSLNodeType s_type = HLSLNodeType::DiscardStatement;
};

struct HLSLBreakStatement : public HLSLStatement
{
    static const HLSLNodeType s_type = HLSLNodeType::BreakStatement;
};

struct HLSLContinueStatement : public HLSLStatement
{
    static const HLSLNodeType s_type = HLSLNodeType::ContinueStatement;
};

struct HLSLIfStatement : public HLSLStatement
{
    static const HLSLNodeType s_type = HLSLNodeType::IfStatement;
    HLSLIfStatement()
    {
        condition     = NULL;
        statement     = NULL;
        elseStatement = NULL;
        isStatic      = false;
    }
    HLSLExpression*     condition;
    HLSLStatement*      statement;
    HLSLStatement*      elseStatement;
    bool                isStatic;
};

struct HLSLForStatement : public HLSLStatement
{
    static const HLSLNodeType s_type = HLSLNodeType::ForStatement;
    HLSLForStatement()
    {
        initialization = NULL;
        condition = NULL;
        increment = NULL;
        statement = NULL;
    }
    HLSLDeclaration*    initialization;
    HLSLExpression*     condition;
    HLSLExpression*     increment;
    HLSLStatement*      statement;
};

struct HLSLBlockStatement : public HLSLStatement
{
    static const HLSLNodeType s_type = HLSLNodeType::BlockStatement;
    HLSLBlockStatement()
    {
        statement = NULL;
    }
    HLSLStatement*      statement;
};


/** Base type for all types of expressions. */
struct HLSLExpression : public HLSLNode
{
    static const HLSLNodeType s_type = HLSLNodeType::Expression;
    HLSLExpression()
    {
        nextExpression = NULL;
    }
    HLSLType            expressionType;
    HLSLExpression*     nextExpression; // Used when the expression is part of a list, like in a function call.

    virtual nlohmann::json      ConvertToJSON(bool bNodeType = true) override;
};

struct HLSLUnaryExpression : public HLSLExpression
{
    static const HLSLNodeType s_type = HLSLNodeType::UnaryExpression;
    HLSLUnaryExpression()
    {
        expression = NULL;
    }
    HLSLUnaryOp         unaryOp;
    HLSLExpression*     expression;
};

struct HLSLBinaryExpression : public HLSLExpression
{
    static const HLSLNodeType s_type = HLSLNodeType::BinaryExpression;
    HLSLBinaryExpression()
    {
        expression1 = NULL;
        expression2 = NULL;
    }
    HLSLBinaryOp        binaryOp;
    HLSLExpression*     expression1;
    HLSLExpression*     expression2;
};

/** ? : construct */
struct HLSLConditionalExpression : public HLSLExpression
{
    static const HLSLNodeType s_type = HLSLNodeType::ConditionalExpression;
    HLSLConditionalExpression()
    {
        condition       = NULL;
        trueExpression  = NULL;
        falseExpression = NULL;
    }
    HLSLExpression*     condition;
    HLSLExpression*     trueExpression;
    HLSLExpression*     falseExpression;
};

struct HLSLCastingExpression : public HLSLExpression
{
    static const HLSLNodeType s_type = HLSLNodeType::CastingExpression;
    HLSLCastingExpression()
    {
        expression = NULL;
    }
    HLSLType            type;
    HLSLExpression*     expression;
};

/** Float, integer, boolean, etc. literal constant. */
struct HLSLLiteralExpression : public HLSLExpression
{
    static const HLSLNodeType s_type = HLSLNodeType::LiteralExpression;
    HLSLBaseType        type;   // Note, not all types can be literals.
    union
    {
        bool            bValue;
        float           fValue;
        int             iValue;
    };
};

/** An identifier, typically a variable name or structure field name. */
struct HLSLIdentifierExpression : public HLSLExpression
{
    static const HLSLNodeType s_type = HLSLNodeType::IdentifierExpression;
    HLSLIdentifierExpression()
    {
        name     = NULL;
        global  = false;
    }
    const char*         name;
    bool                global; // This is a global variable.
};

/** float2(1, 2) */
struct HLSLConstructorExpression : public HLSLExpression
{
    static const HLSLNodeType s_type = HLSLNodeType::ConstructorExpression;
	HLSLConstructorExpression()
	{
		argument = NULL;
	}
    HLSLType            type;
    HLSLExpression*     argument;
};

/** object.member **/
struct HLSLMemberAccess : public HLSLExpression
{
    static const HLSLNodeType s_type = HLSLNodeType::MemberAccess;
	HLSLMemberAccess()
	{
		object  = NULL;
		field   = NULL;
		swizzle = false;
	}
    HLSLExpression*     object;
    const char*         field;
    bool                swizzle;
};

/** array[index] **/
struct HLSLArrayAccess : public HLSLExpression
{
    static const HLSLNodeType s_type = HLSLNodeType::ArrayAccess;
	HLSLArrayAccess()
	{
		array = NULL;
		index = NULL;
	}
    HLSLExpression*     array;
    HLSLExpression*     index;
};

struct HLSLFunctionCall : public HLSLExpression
{
    static const HLSLNodeType s_type = HLSLNodeType::FunctionCall;
	HLSLFunctionCall()
	{
		function     = NULL;
		argument     = NULL;
		numArguments = 0;
	}
    const HLSLFunction* function;
    HLSLExpression*     argument;
    int                 numArguments;
};

struct HLSLStateAssignment : public HLSLNode
{
    static const HLSLNodeType s_type = HLSLNodeType::StateAssignment;
    HLSLStateAssignment()
    {
        stateName = NULL;
        sValue = NULL;
        nextStateAssignment = NULL;
    }

    const char*             stateName;
    int                     d3dRenderState;
    union {
        int                 iValue;
        float               fValue;
        const char *        sValue;
    };
    HLSLStateAssignment*    nextStateAssignment;
};

struct HLSLSamplerState : public HLSLExpression // @@ Does this need to be an expression? Does it have a type? I guess type is useful.
{
    static const HLSLNodeType s_type = HLSLNodeType::SamplerState;
    HLSLSamplerState()
    {
        numStateAssignments = 0;
        stateAssignments = NULL;
    }

    int                     numStateAssignments;
    HLSLStateAssignment*    stateAssignments;
};

struct HLSLPass : public HLSLNode
{
    static const HLSLNodeType s_type = HLSLNodeType::Pass;
    HLSLPass()
    {
        name = NULL;
        numStateAssignments = 0;
        stateAssignments = NULL;
        nextPass = NULL;
    }
    
    const char*             name;
    int                     numStateAssignments;
    HLSLStateAssignment*    stateAssignments;
    HLSLPass*               nextPass;
};

struct HLSLTechnique : public HLSLStatement
{
    static const HLSLNodeType s_type = HLSLNodeType::Technique;
    HLSLTechnique()
    {
        name = NULL;
        numPasses = 0;
        passes = NULL;
    }

    const char*         name;
    int                 numPasses;
    HLSLPass*           passes;
};

struct HLSLPipeline : public HLSLStatement
{
    static const HLSLNodeType s_type = HLSLNodeType::Pipeline;
    HLSLPipeline()
    {
        name = NULL;
        numStateAssignments = 0;
        stateAssignments = NULL;
    }
    
    const char*             name;
    int                     numStateAssignments;
    HLSLStateAssignment*    stateAssignments;
};

struct HLSLStage : public HLSLStatement
{
    static const HLSLNodeType s_type = HLSLNodeType::Stage;
    HLSLStage()
    {
        name = NULL;
        statement = NULL;
        inputs = NULL;
        outputs = NULL;
    }

    const char*             name;
    HLSLStatement*          statement;
    HLSLDeclaration*        inputs;
    HLSLDeclaration*        outputs;
};


/**
 * Abstract syntax tree for parsed HLSL code.
 */
class HLSLTree
{

public:

    explicit HLSLTree();

    /** Adds a string to the string pool used by the tree. */
    const char* AddString(const char* string);
    const char* AddStringFormat(const char* string, ...);

    /** Returns true if the string is contained within the tree. */
    bool GetContainsString(const char* string) const;

    /** Returns the root block in the tree */
    HLSLRoot* GetRoot() const;

    /** Adds a new node to the tree with the specified type. */
    template <class T>
    T* AddNode(const char* fileName, int line)
    {
        HLSLNode* node = new (AllocateMemory(sizeof(T))) T();
        node->nodeType  = T::s_type;
        node->fileName  = fileName;
        node->line      = line;
        return static_cast<T*>(node);
    }

    HLSLFunction * FindFunction(const char * name);
    HLSLDeclaration * FindGlobalDeclaration(const char * name, HLSLBuffer ** buffer_out = NULL);
    HLSLStruct * FindGlobalStruct(const char * name);
    HLSLTechnique * FindTechnique(const char * name);
    HLSLPipeline * FindFirstPipeline();
    HLSLPipeline * FindNextPipeline(HLSLPipeline * current);
    HLSLPipeline * FindPipeline(const char * name);
    HLSLBuffer * FindBuffer(const char * name);

    bool GetExpressionValue(HLSLExpression * expression, int & value);
    int GetExpressionValue(HLSLExpression * expression, float values[4]);

    bool NeedsFunction(const char * name);

private:

    void* AllocateMemory(size_t size);
    void  AllocatePage();

private:

    static const size_t s_nodePageSize = 1024 * 4;

    struct NodePage
    {
        NodePage*   next = NULL;
        char        buffer[s_nodePageSize];
    };

    StringPool      m_stringPool;
    HLSLRoot*       m_root;

    NodePage*       m_firstPage;
    NodePage*       m_currentPage;
    size_t          m_currentPageOffset;

    std::vector<NodePage> m_NodePages;
};



class HLSLTreeVisitor
{
public:
    virtual void VisitType(HLSLType & type);

    virtual void VisitRoot(HLSLRoot * node);
    virtual void VisitTopLevelStatement(HLSLStatement * node);
    virtual void VisitStatements(HLSLStatement * statement);
    virtual void VisitStatement(HLSLStatement * node);
    virtual void VisitDeclaration(HLSLDeclaration * node);
    virtual void VisitStruct(HLSLStruct * node);
    virtual void VisitStructField(HLSLStructField * node);
    virtual void VisitBuffer(HLSLBuffer * node);
    //virtual void VisitBufferField(HLSLBufferField * node);
    virtual void VisitFunction(HLSLFunction * node);
    virtual void VisitArgument(HLSLArgument * node);
    virtual void VisitExpressionStatement(HLSLExpressionStatement * node);
    virtual void VisitExpression(HLSLExpression * node);
    virtual void VisitReturnStatement(HLSLReturnStatement * node);
    virtual void VisitDiscardStatement(HLSLDiscardStatement * node);
    virtual void VisitBreakStatement(HLSLBreakStatement * node);
    virtual void VisitContinueStatement(HLSLContinueStatement * node);
    virtual void VisitIfStatement(HLSLIfStatement * node);
    virtual void VisitForStatement(HLSLForStatement * node);
    virtual void VisitBlockStatement(HLSLBlockStatement * node);
    virtual void VisitUnaryExpression(HLSLUnaryExpression * node);
    virtual void VisitBinaryExpression(HLSLBinaryExpression * node);
    virtual void VisitConditionalExpression(HLSLConditionalExpression * node);
    virtual void VisitCastingExpression(HLSLCastingExpression * node);
    virtual void VisitLiteralExpression(HLSLLiteralExpression * node);
    virtual void VisitIdentifierExpression(HLSLIdentifierExpression * node);
    virtual void VisitConstructorExpression(HLSLConstructorExpression * node);
    virtual void VisitMemberAccess(HLSLMemberAccess * node);
    virtual void VisitArrayAccess(HLSLArrayAccess * node);
    virtual void VisitFunctionCall(HLSLFunctionCall * node);
    virtual void VisitStateAssignment(HLSLStateAssignment * node);
    virtual void VisitSamplerState(HLSLSamplerState * node);
    virtual void VisitPass(HLSLPass * node);
    virtual void VisitTechnique(HLSLTechnique * node);
    virtual void VisitPipeline(HLSLPipeline * node);


    virtual void VisitFunctions(HLSLRoot * root);
    virtual void VisitParameters(HLSLRoot * root);

    HLSLFunction * FindFunction(HLSLRoot * root, const char * name);
    HLSLDeclaration * FindGlobalDeclaration(HLSLRoot * root, const char * name);
    HLSLStruct * FindGlobalStruct(HLSLRoot * root, const char * name);
};


// Tree transformations:
extern void PruneTree(HLSLTree* tree, const char* entryName0, const char* entryName1 = NULL);
extern void SortTree(HLSLTree* tree);
extern void GroupParameters(HLSLTree* tree);
extern void HideUnusedArguments(HLSLFunction * function);
extern bool EmulateAlphaTest(HLSLTree* tree, const char* entryName, float alphaRef = 0.5f);
extern void FlattenExpressions(HLSLTree* tree);
    
} // M4

#endif
