#include "engine.h"

using namespace bzmag;
using namespace bzmag::engine;

//-----------------------------------------------------------------------------
void initialize_Engine(Module* module)
{
    REGISTER_TYPE(module, GeomBaseNode);
    REGISTER_TYPE(module, GeomBooleanNode);
    REGISTER_TYPE(module, GeomCurveNode);
    REGISTER_TYPE(module, GeomHeadNode);
    REGISTER_TYPE(module, GeomCircleNode);
    REGISTER_TYPE(module, GeomRectNode);
    REGISTER_TYPE(module, GeomCoverLineNode);
    REGISTER_TYPE(module, GeomCloneToNode);
    REGISTER_TYPE(module, GeomCloneFromNode);
    REGISTER_TYPE(module, GeomSubtractNode);
    REGISTER_TYPE(module, GeomMoveNode);
    REGISTER_TYPE(module, GeomRotateNode);

	REGISTER_TYPE(module, CSNode);
	REGISTER_TYPE(module, MaterialNode);
	REGISTER_TYPE(module, BCNode);

    REGISTER_TYPE(module, Expression);
    REGISTER_TYPE(module, ExpressionServer);
    
    REGISTER_TYPE(module, GeomToPolyTriangle);

    GeomToPolyTriangle::setSingletonPath("sys/server/triangle");
    ExpressionServer::setSingletonPath("sys/server/expressions");
}

//-----------------------------------------------------------------------------
void finalize_Engine(Module* module)
{
    
}

//-----------------------------------------------------------------------------
DECLARE_MODULE(Engine);
