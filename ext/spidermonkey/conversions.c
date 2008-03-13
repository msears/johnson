#include "conversions.h"

static jsval convert_float_or_bignum_to_js(OurContext* context, VALUE float_or_bignum)
{
  jsval js;
  assert(JS_NewDoubleValue(context->js, NUM2DBL(float_or_bignum), &js));
  return js;
}

jsval convert_to_js(OurContext* context, VALUE ruby)
{
  switch(TYPE(ruby))
  {
    case T_NIL:
      return JSVAL_NULL;

  	case T_TRUE:
      return JSVAL_TRUE;
    
  	case T_FALSE:
      return JSVAL_FALSE;

  	case T_STRING:
  	  return STRING_TO_JSVAL(JS_NewStringCopyZ(context->js, StringValuePtr(ruby)));

  	case T_FIXNUM:
    	return INT_TO_JSVAL(NUM2INT(ruby));

  	case T_FLOAT:
  	case T_BIGNUM:
      return convert_float_or_bignum_to_js(context, ruby);

    // UNIMPLEMENTED BELOW THIS LINE

  	case T_OBJECT:
  	case T_CLASS:
  	case T_FILE:
  	case T_STRUCT:
  	case T_MODULE:
  	case T_REGEXP:
  	case T_ARRAY:
  	case T_HASH:
  	case T_DATA:
  	case T_SYMBOL: 
    
    default:
      Johnson_Error_raise("unknown ruby type in switch");
  }
  
  return JSVAL_NULL;
}

VALUE convert_to_ruby(OurContext* context, jsval js)
{
  switch (JS_TypeOfValue(context->js, js))
  {
    case JSTYPE_VOID:
      return Qnil;
      
    case JSTYPE_OBJECT:
      if (JSVAL_NULL == js) return Qnil;
      // FIXME: this is only implemented for nil

    case JSTYPE_BOOLEAN:
      return JSVAL_TRUE == js ? Qtrue : Qfalse;
      
    case JSTYPE_STRING:
      return rb_str_new2(JS_GetStringBytes(JSVAL_TO_STRING(js)));  
      
    case JSTYPE_NUMBER:
      if (JSVAL_IS_INT(js)) return INT2FIX(JSVAL_TO_INT(js));
      else return rb_float_new(*JSVAL_TO_DOUBLE(js));

    // UNIMPLEMENTED BELOW THIS LINE

    case JSTYPE_FUNCTION:
      
    default:
      Johnson_Error_raise("unknown js type in switch");
  }
  
  return Qnil;
}