#include <cctype>
#include <cstdio>
#include <string>
#include <vector>
#include <map>


enum Token {
	tok_eof = -1,

	// Rust keywords
	tok_as = -2,				// 执行原始类型转换，消除包含项目的特定特征的歧义，或在useand extern crate语句中重命名项目
	tok_async = -3,				// 返回a Future而不是阻塞当前线程
	tok_await = -4,				// 暂停执行直到a的结果Future准备好
	tok_break = -5,				// 立即退出循环
	tok_const = -6,				// 定义常量项目或常量原始指针
	tok_continue = -7,			// 继续进行下一个循环迭代
	tok_crate = -8,				// 链接一个外部板条箱或一个宏变量，该变量代表定义宏的板条箱
	tok_dyn = -9,				// 动态分配给特征对象
	tok_else = -10,				// 回退if并if let控制流程构造
	tok_enum = -11,				// 定义一个枚举
	tok_extern = -12,			// 链接外部板条箱，函数或变量
	tok_false = -13,			// 布尔值假
	tok_fn = -14,				// 定义函数或函数指针类型
	tok_for = -15,				// 循环遍历迭代器中的项目，实现特征或指定更高级别的生存期
	tok_if = -16,				// 根据条件表达式的结果进行分支
	tok_impl = -17,				// 实现固有或特征功能
	tok_in = -18,				// for循环语法的一部分
	tok_let = -19,				// 绑定变量
	tok_loop = -20,				// 无条件循环
	tok_match = -21,			// 将值匹配到模式
	tok_mod = -22,				// 定义一个模块
	tok_move = -23,				// 使封包对其所有捕获物拥有所有权
	tok_mut = -24,				// 表示引用，原始指针或模式绑定中的可变性
	tok_pub = -25,				// 表示结构字段，impl块或模块中的公共可见性
	tok_ref = -26,				// 通过引用绑定
	tok_return = -27,			// 从函数返回
	tok_Self = -28,				// 我们正在定义或实现的类型的类型别名
	tok_self = -29,				// 方法主题或当前模块
	tok_static = -30,			// 持续整个程序执行的全局变量或生命周期
	tok_struct = -31,			// 定义一个结构
	tok_super = -32,			// 当前模块的父模块
	tok_trait = -33,			// 定义一个特征
	tok_true = -34,				// 布尔值真
	tok_type = -35,				// 定义类型别名或关联的类型
	tok_unsafe = -36,			// 表示不安全的代码，功能，特征或实现
	tok_use = -37,				// 将符号纳入范围
	tok_where = -38,			// 表示约束类型的子句
	tok_while = -39,			// 根据表达式的结果有条件地循环

	// Primary
	tok_identifier = -40,
	tok_number = -41,

	//Rust primitive types
	tok_array = -42,			// A fixed - size array, denoted[T; N], for the element type, T, and the non - negative compile - time constant size, N.
	tok_bool = -43,				// The boolean type.
	tok_char = -44,				// A character type.
	tok_f32 = -45,				// A 32 - bit floating point type(specifically, the "binary32" type defined in IEEE 754 - 2008).
	tok_f64	= -46,				// A 64 - bit floating point type(specifically, the "binary64" type defined in IEEE 754 - 2008).
	tok_i8 = -47,				// The 8 - bit signed integer type.
	tok_i16 = -48,				// The 16 - bit signed integer type.
	tok_i32 = -49,				// The 32 - bit signed integer type.
	tok_i64 = -50,				// The 64 - bit signed integer type.
	tok_i128 = -51,				// The 128 - bit signed integer type.
	tok_isize = -52,			// The pointer - sized signed integer type.
	tok_pointer = -53,			// Raw, unsafe pointers, *const T, and *mut T.
	tok_reference = -54,		// References, both shared and mutable.
	tok_slice = -55,			// A dynamically - sized view into a contiguous sequence, [T].Contiguous here means that elements are laid out so that every element is the same distance from its neighbors.
	tok_str	= -56,				// String slices.
	tok_tuple = -57,			// A finite heterogeneous sequence, (T, U, ..).
	tok_u8 = -58,				// The 8 - bit unsigned integer type.
	tok_u16 = -59,				// The 16 - bit unsigned integer type.
	tok_u32 = -60,				// The 32 - bit unsigned integer type.
	tok_u64	= -61,				// The 64 - bit unsigned integer type.
	tok_u128 = -62,				// The 128 - bit unsigned integer type.
	tok_unit = -63,				// The() type, also called "unit".
	tok_usize = -64				// The pointer - sized unsigned integer type.
};


static const int rustPriTypesMax = -42;
static const int rustPriTypesMin = -64;
static std::string IdentifierStr; // Filled in if tok_identifier
static double NumVal;             // Filled in if tok_number

/// gettok - Return the next token from standard input.
static int gettok() {
	static int LastChar = ' ';

	// Skip any whitespace.
	while (isspace(LastChar))
		LastChar = getchar();

	if (isalpha(LastChar)) { // identifier: [a-zA-Z][a-zA-Z0-9]*
		IdentifierStr = LastChar;
		while (isalnum((LastChar = getchar())))
			IdentifierStr += LastChar;
		//Keywords
		if (IdentifierStr == "as")
			return tok_as;
		if (IdentifierStr == "async")
			return tok_async;
		if (IdentifierStr == "await")
			return tok_async;
		if (IdentifierStr == "break")
			return tok_break;
		if (IdentifierStr == "const")
			return tok_const;
		if (IdentifierStr == "continue")
			return tok_continue;
		if (IdentifierStr == "crate")
			return tok_crate;
		if (IdentifierStr == "dyn")
			return tok_dyn;
		if (IdentifierStr == "else")
			return tok_else;
		if (IdentifierStr == "enum")
			return tok_enum;
		if (IdentifierStr == "extern")
			return tok_extern;
		if (IdentifierStr == "false")
			return tok_false;
		if (IdentifierStr == "fn")
			return tok_fn;
		if (IdentifierStr == "for")
			return tok_for;
		if (IdentifierStr == "if")
			return tok_if;
		if (IdentifierStr == "impl")
			return tok_impl;
		if (IdentifierStr == "in")
			return tok_in;
		if (IdentifierStr == "let")
			return tok_let;
		if (IdentifierStr == "loop")
			return tok_loop;
		if (IdentifierStr == "match")
			return tok_match;
		if (IdentifierStr == "mod")
			return tok_mod;
		if (IdentifierStr == "move")
			return tok_move;
		if (IdentifierStr == "mut")
			return tok_mut;
		if (IdentifierStr == "pub")
			return tok_pub;
		if (IdentifierStr == "ref")
			return tok_ref;
		if (IdentifierStr == "return")
			return tok_return;
		if (IdentifierStr == "Self")
			return tok_Self;
		if (IdentifierStr == "self")
			return tok_self;
		if (IdentifierStr == "static")
			return tok_static;
		if (IdentifierStr == "struct")
			return tok_struct;
		if (IdentifierStr == "super")
			return tok_super;
		if (IdentifierStr == "trait")
			return tok_trait;
		if (IdentifierStr == "true")
			return tok_true;
		if (IdentifierStr == "type")
			return tok_type;
		if (IdentifierStr == "unsafe")
			return tok_unsafe;
		if (IdentifierStr == "use")
			return tok_use;
		if (IdentifierStr == "where")
			return tok_where;
		if (IdentifierStr == "while")
			return tok_while;


		//Primitive types
		if (IdentifierStr == "array")
			return tok_array;
		if (IdentifierStr == "bool")
			return tok_bool;
		if (IdentifierStr == "char")
			return tok_char;
		if (IdentifierStr == "f32")
			return tok_f32;
		if (IdentifierStr == "f64")
			return tok_f64;
		if (IdentifierStr == "i8")
			return tok_i8;
		if (IdentifierStr == "i16")
			return tok_i16;
		if (IdentifierStr == "i32")
			return tok_i32;
		if (IdentifierStr == "i64")
			return tok_i64;
		if (IdentifierStr == "i128")
			return tok_i128;
		if (IdentifierStr == "isize")
			return tok_isize;
		if (IdentifierStr == "pointer")
			return tok_pointer;
		if (IdentifierStr == "reference")
			return tok_reference;
		if (IdentifierStr == "slice")
			return tok_slice;
		if (IdentifierStr == "str")
			return tok_str;
		if (IdentifierStr == "tuple")
			return tok_tuple;
		if (IdentifierStr == "u8")
			return tok_u8;
		if (IdentifierStr == "u16")
			return tok_u16;
		if (IdentifierStr == "u32")
			return tok_u32;
		if (IdentifierStr == "u64")
			return tok_u64;
		if (IdentifierStr == "u128")
			return tok_u128;
		if (IdentifierStr == "unit")
			return tok_unit;
		if (IdentifierStr == "usize")
			return tok_usize;


		return tok_identifier;
	}

	if (isdigit(LastChar) || LastChar == '.') { // Number: [0-9.]+
		std::string NumStr;
		do {
			NumStr += LastChar;
			LastChar = getchar();
		} while (isdigit(LastChar) || LastChar == '.');

		NumVal = strtod(NumStr.c_str(), nullptr);
		return tok_number;
	}

	if (LastChar == '//') {
		// Comment until end of line.
		do
			LastChar = getchar();
		while (LastChar != EOF && LastChar != '\n' && LastChar != '\r');

		if (LastChar != EOF)
			return gettok();
	}

	if (LastChar == '/*'){
		// Normol comment.
		do
			LastChar = getchar();
		while (LastChar != EOF && (LastChar != '*'&&getchar() != '/'));

		if (LastChar != EOF)
			return gettok();
	}

	// Check for end of file.  Don't eat the EOF.
	if (LastChar == EOF)
		return tok_eof;

	// Otherwise, just return the character as its ascii value.
	int ThisChar = LastChar;
	LastChar = getchar();
	return ThisChar;
}