#pragma once

/* Token types */
namespace token
{
    enum
    {
        kw_class = 1,
        kw_else,
        kw_false,
        kw_fi,
        kw_if,
        kw_in,
        kw_inherits,
        kw_isvoid,
        kw_let,
        kw_loop,
        kw_pool,
        kw_then,
        kw_while,
        kw_case,
        kw_esac,
        kw_new,
        kw_of,
        kw_not,
        kw_true,

        bc_Object,
        bc_Int,
        bc_String,
        bc_Bool,

        identifier,

        self_type,
        self,

        expr_assignment,

        binop_plus,
        binop_minus,
        binop_mul,
        binop_div,
        binop_l,
        binop_le,
        binop_eq,

        l_brace,
        r_brace,
        l_paren,
        r_paren,

        colon,
        semicolon,
        dot,
        comma,

        lit_string,
        lit_int,

        UNKNOWN,
    };
} // namespace token
