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

#ifndef VRAY_FOR_HOUDINI_EXPORTER_H
#define VRAY_FOR_HOUDINI_EXPORTER_H

#include "vfh_defines.h"
#include "vfh_typedefs.h"
#include "vfh_vray.h"
#include "vfh_plugin_exporter.h"
#include "vfh_plugin_info.h"

#include <OP/OP_Node.h>
#include <OBJ/OBJ_Node.h>


namespace VRayForHoudini {

typedef VUtils::HashMap<int> SHOPToID;

enum VRayLightType {
	VRayLightOmni      = 0,
	VRayLightRectangle = 2,
	VRayLightSphere    = 4,
	VRayLightDome      = 6,
	VRayLightSun       = 7,
};

class VRayExporter
{
public:
	typedef std::set<VRayExporter*> ExporterInstances;
	static ExporterInstances        Instances;

	VRayExporter();
	~VRayExporter();

public:
	int                     init(int mode);

	int                     exportScene();

	int                     exportView(OP_Node *rop);
	void                    exportCamera(OP_Node *camera);

	int                     exportSettings(OP_Node *rop);
	void                    exportRenderChannels(OP_Node *op_node);

	void                    exportEnvironment(OP_Node *op_node);
	void                    exportEffects(OP_Node *op_net);

	void                    exportDone();

	VRay::Plugin            exportObject(OBJ_Node *obj_node);
	VRay::Plugin            exportNode(OBJ_Node *obj_node, VRay::Plugin material, VRay::Plugin geometry);
	VRay::Plugin            exportNodeData(SOP_Node *sop_node, SHOPToID &shopToID);

	VRay::Plugin            exportParticles(OBJ_Node *dop_network);

	VRay::Plugin            exportLight(OBJ_Node *obj_node);

	VRay::Plugin            exportMaterial(SHOP_Node *shop_node);
	VRay::Plugin            exportVop(OP_Node *op_node);

#ifdef CGR_HAS_VRAYSCENE
	VRay::Plugin            exportVRayScene(OBJ_Node *obj_node, SOP_Node *geom_node);
#endif

	VRay::Plugin            exportPlugin(const Attrs::PluginDesc &pluginDesc);

	int                     renderFrame(int locked=false);
	int                     renderSequence(int start, int end, int step, int locked=false);
	int                     exportScene(const std::string &filepath);

	void                    setAnimation(bool on);
	void                    setFrame(float frame);
	void                    setRop(OP_Node *rop) { m_rop = rop; }
	void                    setMode(int mode) { m_renderMode = mode; }
	void                    setContext(const OP_Context &ctx) { m_context = ctx; }

	void                    setRenderSize(int w, int h);

	OP_Context             &getContext()  { return m_context;   }
	VRayPluginRenderer     *getRenderer() { return &m_renderer; }
	OP_Node                *getRop()      { return m_rop; }

	VRay::Plugin            exportGeomStaticMesh(SOP_Node *sop_node, const GU_Detail *gdp, SHOPToID &shopToID);
	VRay::Plugin            exportGeomMayaHair(SOP_Node *sop_node, const GU_Detail *gdp);
	void                    exportGeomMayaHairGeom(SOP_Node *sop_node, const GU_Detail *gdp, Attrs::PluginDesc &pluginDesc);

public:
	static OP_Input               *getConnectedInput(OP_Node *op_node, const std::string &inputName);
	static OP_Node                *getConnectedNode(OP_Node *op_node, const std::string &inputName);
	static const Parm::SocketDesc *getConnectedOutputType(OP_Node *op_node, const std::string &inputName);

public:
	static VRay::Transform  GetOBJTransform(OBJ_Node *obj_node, OP_Context &context, bool flip=false);
	static void             GetOBJTransform(OBJ_Node *obj_node, OP_Context &context, float tm[4][4]);
	static void             TransformToMatrix4(const VUtils::TraceTransform &tm, UT_Matrix4 &m);
	static VRay::Transform  Matrix4ToTransform(const UT_Matrix4D &m4, bool flip=false);

	static OP_Node         *GetCamera(OP_Node *rop);
	static OP_Node         *FindChildNodeByType(OP_Node *op_node, const std::string &op_type);

	bool                    setAttrValueFromOpNode(Attrs::PluginDesc &plugin, const Parm::AttrDesc &parmDesc, OP_Node *opNode, bool checkPrefix=false);
	int                     setAttrsFromOpNode(Attrs::PluginDesc &plugin, OP_Node *opNode, bool checkPrefix=false, const std::string &prefix="");

	VRay::Plugin            exportConnectedVop(OP_Node *op_node, const UT_String &inputName);

private:
	VRayPluginRenderer      m_renderer;
	OP_Context              m_context;
	OP_Node                *m_rop;
	int                     m_renderMode;
	int                     m_is_aborted;

public:
	void                    phxAddSimumation(VRay::Plugin sim);
	VRay::ValueList         m_phxSimulations;

public:
	static bool             TraverseOBJs(OP_Node &op_node, void *data);
	static void             TraverseOBJ(OBJ_Node *obj_node, void *data);

	void                    addOpCallback(OP_Node *op_node, OP_EventMethod cb);
	void                    delOpCallback(OP_Node *op_node, OP_EventMethod cb);
	void                    resetOpCallbacks();

	int                     isRt();
	int                     isRtRunning();
	int                     isAborted();

	void                    addAbortCallback();
	void                    addRtCallbacks();
	void                    removeRtCallbacks();

	static void             RtCallbackObjManager(OP_Node *caller, void *callee, OP_EventType type, void *data);
	static void             RtCallbackLight(OP_Node *caller, void *callee, OP_EventType type, void *data);
	static void             RtCallbackNode(OP_Node *caller, void *callee, OP_EventType type, void *data);
	static void             RtCallbackNodeData(OP_Node *caller, void *callee, OP_EventType type, void *data);
	static void             RtCallbackView(OP_Node *caller, void *callee, OP_EventType type, void *data);
	static void             RtCallbackVop(OP_Node *caller, void *callee, OP_EventType type, void *data);

	static void             CallbackSequence();

};

} // namespace VRayForHoudini

#endif // VRAY_FOR_HOUDINI_EXPORTER_H
