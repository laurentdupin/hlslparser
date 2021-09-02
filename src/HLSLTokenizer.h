#ifndef HLSL_TOKENIZER_H
#define HLSL_TOKENIZER_H

namespace M4
{

/** In addition to the values in this enum, all of the ASCII characters are
valid tokens. */
enum class HLSLToken
{
    // Built-in types.
    Float         = 256,
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
    Texture,
    Sampler,
    Sampler2D,
    Sampler3D,
    SamplerCube,
    Sampler2DShadow,
    Sampler2DMS,
    Sampler2DArray,

    // Reserved words.
    If,
    Else,
    For,
    While,
    Break,
    True,
    False,
    Void,
    Struct,
    CBuffer,
    TBuffer,
    Register,
    Return,
    Continue,
    Discard,
    Const,
    Static,
    Inline,

    // Input modifiers.
    Uniform,
    In,
    Out,
    InOut,

    // Effect keywords.
    SamplerState,
    Technique,
    Pass,

    // Multi-character symbols.
    LessEqual,
    GreaterEqual,
    EqualEqual,
    NotEqual,
    PlusPlus,
    MinusMinus,
    PlusEqual,
    MinusEqual,
    TimesEqual,
    DivideEqual,
    AndAnd,       // &&
    BarBar,       // ||
    
    // Other token types.
    FloatLiteral,
	HalfLiteral,
    IntLiteral,
    Identifier,

    EndOfStream,
};

class HLSLTokenizer
{

public:

    /// Maximum string length of an identifier.
    static const int s_maxIdentifier = 255 + 1;

    /** The file name is only used for error reporting. */
    HLSLTokenizer(const char* fileName, const char* buffer, size_t length);

    /** Advances to the next token in the stream. */
    void Next();

    /** Returns the current token in the stream. */
    int GetToken() const;

    /** Returns the number of the current token. */
    float GetFloat() const;
    int   GetInt() const;

    /** Returns the identifier for the current token. */
    const char* GetIdentifier() const;

    /** Returns the line number where the current token began. */
    int GetLineNumber() const;

    /** Returns the file name where the current token began. */
    const char* GetFileName() const;

    /** Gets a human readable text description of the current token. */
    void GetTokenName(char buffer[s_maxIdentifier]) const;

    /** Reports an error using printf style formatting. The current line number
    is included. Only the first error reported will be output. */
    void Error(const char* format, ...);

    /** Gets a human readable text description of the specified token. */
    static void GetTokenName(int token, char buffer[s_maxIdentifier]);

private:

    bool SkipWhitespace();
    bool SkipComment();
	bool SkipPragmaDirective();
    bool ScanNumber();
    bool ScanLineDirective();

private:

    const char*         m_fileName;
    const char*         m_buffer;
    const char*         m_bufferEnd;
    int                 m_lineNumber;
    bool                m_error;

    int                 m_token;
    float               m_fValue;
    int                 m_iValue;
    char                m_identifier[s_maxIdentifier];
    char                m_lineDirectiveFileName[s_maxIdentifier];
    int                 m_tokenLineNumber;

};

}

#endif
