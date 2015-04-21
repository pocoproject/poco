#ifndef Foundation_Compiler_GCC_INCLUDED
#define Foundation_Compiler_GCC_INCLUDED


// 
// GCC diagnostics enable/disable by Patrick Horgan, see
// http://dbp-consulting.com/tutorials/SuppressingGCCWarnings.html
// use example: GCC_DIAG_OFF(unused-variable)
// 
#ifdef __GNUC__
	#if defined(POCO_COMPILER_GCC) && (((__GNUC__ * 100) + __GNUC_MINOR__) >= 406) && !defined(POCO_NO_GCC_DIAG)
		#ifdef GCC_DIAG_OFF
			#undef GCC_DIAG_OFF
		#endif
		#ifdef GCC_DIAG_ON
			#undef GCC_DIAG_ON
		#endif
		#define GCC_DIAG_STR(s) #s
		#define GCC_DIAG_JOINSTR(x,y) GCC_DIAG_STR(x ## y)
		#define GCC_DIAG_DO_PRAGMA(x) _Pragma (#x)
		#define GCC_DIAG_PRAGMA(x) GCC_DIAG_DO_PRAGMA(GCC diagnostic x)
		#if ((__GNUC__ * 100) + __GNUC_MINOR__) >= 406
			#define GCC_DIAG_OFF(x) GCC_DIAG_PRAGMA(push) \
			GCC_DIAG_PRAGMA(ignored GCC_DIAG_JOINSTR(-W,x))
			#define GCC_DIAG_ON(x) GCC_DIAG_PRAGMA(pop)
		#else
			#define GCC_DIAG_OFF(x) GCC_DIAG_PRAGMA(ignored GCC_DIAG_JOINSTR(-W,x))
			#define GCC_DIAG_ON(x)  GCC_DIAG_PRAGMA(warning GCC_DIAG_JOINSTR(-W,x))
		#endif
	#else
		#define GCC_DIAG_OFF(x)
		#define GCC_DIAG_ON(x)
	#endif
#endif // __GNUC__


#endif // Foundation_Compiler_GCC_INCLUDED