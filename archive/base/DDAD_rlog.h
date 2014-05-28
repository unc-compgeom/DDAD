/**
 * Filename: DDAD_rlog.h
 * Project name: degDrivenAlgoDesig
 * Created on: 1 Oct 2010
 * Author: David L. Millman <dave@cs.unc.edu>
 */

#ifndef _DDAD_DDAD_RLOG_H_
#define _DDAD_DDAD_RLOG_H_




#ifdef DDAD_LOGGING_ON
    #define RLOG_COMPONENT DDAD

    #include <iostream>
    #include <string>
    #include <sstream>
    #include <cassert>
    #include <stdio.h>

    #include <rlog/rlog.h>
    #include <rlog/RLogChannel.h>
    #include <rlog/StdioNode.h>
    #include <rlog/RLogNode.h>
    #undef RLOG_SECTION
    #define RLOG_SECTION

    using namespace rlog;

    template <typename Thing>
    std::string thing_to_string(const Thing &t) {
        std::ostringstream os;
        os << t;
        return os.str();
    }

    template <typename Thing>
    std::string array_to_string(const Thing* a, int n) {
        std::ostringstream os;
        os << "{ ";
        for (int i = 0 ; i < n ; ++i) { os << a[i] << " "; }
        os << "}";
        return os.str();
    }

    template <class Iterator>
    std::string vect_to_string(Iterator begin, Iterator end,
            const std::string pre="\t", const std::string post="\n",
            bool count=false) {
        std::ostringstream os;
        int i = 0;
        for (Iterator iter = begin ; iter != end ; ++iter, ++i) {
            os << pre;
            if (count) { os << i << ".."; }
            os << *iter << post;
        }
        return os.str();
    }

    template <typename Iterator>
    std::string vect_of_ptrs_to_string(Iterator begin, Iterator end,
            const std::string pre="\t", const std::string post="\n",
            bool count=false) {
        std::ostringstream os;
        int i = 0;
        for (Iterator iter = begin ; iter != end ; ++iter, ++i) {
            os << pre;
            if (count) { os << i << ".."; }
            os << **iter << post;
        }
        return os.str();
    }


    template <class Iterator>
    std::string pair_vect_to_string(Iterator begin, Iterator end,
            const std::string pre="\t", const std::string post="\n",
            bool count=false) {
        std::ostringstream os;
        int i = 0;
        for (Iterator iter = begin ; iter != end ; ++iter, ++i) {
            os << pre;
            if (count) { os << i << ".."; }
            os << "(" << iter->first << ", " << iter->second << ")" << post;
        }
        return os.str();
    }

    template <class Iterator>
    std::string pair_vect_x_ptr_to_string(Iterator begin, Iterator end,
            const std::string pre="\t", const std::string post="\n",
            bool count=false) {
        std::ostringstream os;
        int i = 0;
        for (Iterator iter = begin ; iter != end ; ++iter) {
            os << pre;
            if (count) { os << i << ".."; }
            os << "(" << iter->first << ", " << *(iter->second) << ")" << post;
        }
        return os.str();
    }

    #define thing_to_cstring(thing) thing_to_string(thing).c_str()

    #define array_to_cstring(array,n) array_to_string(array,n).c_str()

    #define vect_to_cstring(begin, end) vect_to_string(begin, end).c_str()

    #define vect_of_ptrs_to_cstring(begin, end) vect_of_ptrs_to_string(begin, end).c_str()

    #define pair_vect_to_cstring(begin, end) pair_vect_to_string(begin, end).c_str()

    #define pair_vect_x_ptr_to_cstring(begin, end) pair_vect_x_ptr_to_string(begin, end).c_str()

    #define ez_rlog_init(argc, argv)                            \
        RLogInit(argc, argv);                                   \
        StdioNode stdlog( STDERR_FILENO );                      \
        stdlog.subscribeTo( GetGlobalChannel( "warning" ));     \
        stdlog.subscribeTo( GetGlobalChannel( "error" ));

    #define rTerminate(msg, exception) rError(msg); throw exception;

    #ifdef NDEBUG
        #undef rAssert
        #define rAssert assert
    #endif

#else

    template <typename Thing>
    std::string thing_to_string(const Thing &t) { return ""; }

    template <typename Thing>
    std::string array_to_string(const Thing* a, int n) { return ""; }

    template <class Iterator>
    std::string vect_to_string(Iterator begin, Iterator end,
            const std::string pre="\t", const std::string post="\n",
            bool count=false) { return ""; }

    template <typename Iterator>
    std::string vect_of_ptrs_to_string(Iterator begin, Iterator end,
            const std::string pre="\t", const std::string post="\n",
            bool count=false) { return ""; }

    template <class Iterator>
    std::string pair_vect_to_string(Iterator begin, Iterator end,
            const std::string pre="\t", const std::string post="\n",
            bool count=false) { return ""; }

    template <class Iterator>
    std::string pair_vect_x_ptr_to_string(Iterator begin, Iterator end,
            const std::string pre="\t", const std::string post="\n",
            bool count=false) { return ""; }

    #define thing_to_cstring(thing) ""

    #define array_to_cstring(array,n) ""

    #define vect_to_cstring(begin, end) ""

    #define vect_of_ptrs_to_cstring(begin, end) ""

    #define pair_vect_to_cstring(begin, end) ""

    #define pair_vect_x_ptr_to_cstring(begin, end) ""

    #define Log_Info "Log_Info"
    #define RLogChannel FILE
    #define DEF_CHANNEL(arg1,arg2) NULL;

    #define ez_rlog_init(argc, argv) \
        printf("********************************************************************************\n"); \
        printf("                                rLog not enabled                                \n"); \
        printf("********************************************************************************\n");

    template <class Str>
    void rWarning_no_op(const Str&, ...) { }

    #define rWarning rWarning_no_op

    template <class Str >
    void rError_no_op(const Str&, ...) { }

    #define rError rError_no_op

    template <class Channel, class Str >
    void rLog_no_op(const Channel&, const Str&, ...) { }

    #define rLog rLog_no_op

    #define rAssert assert
    #define rTerminate(msg, exception) printf(msg); throw exception;

#endif

#define varDump(thing) std::cout << "***DEBUG***: (" << &(__FILE__[PATH_TRIM]) << ":" << __LINE__ << ") " << #thing << " = " << thing << std::endl;

#endif
