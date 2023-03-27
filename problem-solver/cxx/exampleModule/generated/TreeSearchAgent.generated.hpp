#include <memory>

#include "sc-memory/sc_memory.hpp"


#include "sc-memory/sc_event.hpp"




#define TreeSearchAgent_hpp_12_init  bool _InitInternal() override \
{ \
    ScMemoryContext ctx(sc_access_lvl_make_min, "TreeSearchAgent::_InitInternal"); \
    bool result = true; \
    return result; \
}


#define TreeSearchAgent_hpp_12_initStatic static bool _InitStaticInternal()  \
{ \
    ScMemoryContext ctx(sc_access_lvl_make_min, "TreeSearchAgent::_InitStaticInternal"); \
    bool result = true; \
    return result; \
}


#define TreeSearchAgent_hpp_12_decl \
private:\
	typedef ScAgent Super;\
protected: \
	TreeSearchAgent(char const * name, sc_uint8 accessLvl) : Super(name, accessLvl) {}\
	virtual sc_result Run(ScAddr const & listenAddr, ScAddr const & edgeAddr, ScAddr const & otherAddr) override; \
private:\
	static std::unique_ptr<ScEvent> ms_event;\
    static std::unique_ptr<ScMemoryContext> ms_context;\
public: \
	static bool handler_emit(ScAddr const & addr, ScAddr const & edgeAddr, ScAddr const & otherAddr)\
	{\
		TreeSearchAgent Instance("TreeSearchAgent", sc_access_lvl_make_min);\
		return Instance.Run(addr, edgeAddr, otherAddr) == SC_RESULT_OK;\
	}\
	static void RegisterHandler()\
	{\
		SC_ASSERT(!ms_event.get(), ());\
		SC_ASSERT(!ms_context.get(), ());\
		ms_context.reset(new ScMemoryContext(sc_access_lvl_make_min, "handler_TreeSearchAgent"));\
		ms_event.reset(new ScEvent(*ms_context, Keynodes::question_find_tree_search, ScEvent::Type::AddOutputEdge, &TreeSearchAgent::handler_emit));\
        if (ms_event.get())\
        {\
            SC_LOG_INFO("Register agent TreeSearchAgent");\
        }\
        else\
        {\
            SC_LOG_ERROR("Can't register agent TreeSearchAgent");\
        }\
	}\
	static void UnregisterHandler()\
	{\
		ms_event.reset();\
		ms_context.reset();\
	}

#define TreeSearchAgent_hpp_TreeSearchAgent_impl \
std::unique_ptr<ScEvent> TreeSearchAgent::ms_event;\
std::unique_ptr<ScMemoryContext> TreeSearchAgent::ms_context;

#undef ScFileID
#define ScFileID TreeSearchAgent_hpp

