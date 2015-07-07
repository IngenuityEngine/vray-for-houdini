//
// Copyright (c) 2015, Chaos Software Ltd
//
// V-Ray For Houdini
//
// Andrei Izrantcev <andrei.izrantcev@chaosgroup.com>
//
// All rights reserved. These coded instructions, statements and
// computer programs contain unpublished information proprietary to
// Chaos Software Ltd, which is protected by the appropriate copyright
// laws and may not be disclosed to third parties or copied or
// duplicated, in whole or in part, without prior written consent of
// Chaos Software Ltd.
//

#include "vop_brdfdiffuse.h"


using namespace VRayForHoudini;


void VOP::BRDFDiffuse::setPluginType()
{
	pluginType = "BRDF";
	pluginID   = "BRDFDiffuse";
}


void VOP::BRDFDiffuse::getCode(UT_String &codestr, const VOP_CodeGenContext &)
{
	codestr.sprintf("Cf = {%.3f,%.3f,%.3f};",
					1.0f, 0.0f, 0.0f);
}
