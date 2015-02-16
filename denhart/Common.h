//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
extern class stOctConv;


//debug 
#define DEBUG 1
#define DEBUG_ERROR(sdebug) fprintf(stderr, "%s", sdebug)
#define DEBUG_CODE(idebug) fprintf(stderr, "%d", idebug)
#define DEBUG_BOOL(sdebug, arg1) fprintf(stderr, "%s : %b", sdebug, arg1)
#define DEBUG_STRING(sdebug, arg1) fprintf(stderr, "%s : %s", sdebug, arg1)
#define DEBUG_INT(sdebug, arg1) fprintf(stderr, "%s : %d", sdebug, arg1)


//errors
#define ERROR_NONE 0
#define ERROR_OCTCONV_OPP_FAILED -1
#define ERROR_OCTCONV_NULL -2
#define ERROR_BIT_OPP_FAILED -3
#define ERROR_BIT_NULL -4
#define ERROR_BIT_OUT_OF_RANGE -5



//common constants
#define REG_12BIT 12
#define REG_7BIT 7
#define REG_5BIT 5
#define REG_3BIT 3


stOctConv OctTable;
