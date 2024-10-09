// 01_raytrace_baseView.h : interface of the CMy01_raytrace_baseView class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once

#include "libgl/CRenderContext.h"
#include "libgl/CRenderer.h"
#include "libgl/CScene.h"
#include "libgl/IGeometryObject_fwd.h"
#include "CTriangleMesh.h"

class CFrameBuffer;
class IShader;
class CTriangleMeshData;

class CRaytraceView : public CWindowImpl<CRaytraceView>
{
	enum
	{
		FRAMEBUFFER_UPDATE_TIMER = 1
	};
public:
	CRaytraceView();
	~CRaytraceView();
	DECLARE_WND_CLASS(NULL)

	BOOL PreTranslateMessage(MSG* pMsg);

	BEGIN_MSG_MAP(CRaytraceView)
		MESSAGE_HANDLER(WM_PAINT, OnPaint)
		MESSAGE_HANDLER(WM_TIMER, OnTimer)
		MESSAGE_HANDLER(WM_CREATE, OnCreate)
		MESSAGE_HANDLER(WM_ERASEBKGND, OnEraseBkgnd)
	END_MSG_MAP()

	// Handler prototypes (uncomment arguments if needed):
	//	LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	//	LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	//	LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)
private:
	LRESULT OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnPaint(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnTimer(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnEraseBkgnd(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);

	void AddSomeTetrahedrons();
	void AddSomeCubes();
	void AddSomeIcosahedrons();
	void AddSomeOctahedrons();

	// Отрисовка содержимого буфера кадра на контексте устройства
	void DrawFrameBuffer(CDC& dc, int x, int y);
	bool UpdateFrameBuffer();

	CSceneObject& AddSceneObject(std::shared_ptr<IGeometryObject const> object, std::shared_ptr<IShader const> shader);
	CSceneObject& AddTetrahedron(std::shared_ptr<IShader const> shader, CMatrix4d const& transform = CMatrix4d(), std::vector<Vertex> const& vertices = {
																													  Vertex({ -1, 0, 1 }),
																													  Vertex({ +1, 0, 1 }),
																													  Vertex({ 0, 0, -1 }),
																													  Vertex({ 0, 2, 0 }),
																												  });
	CSceneObject& AddCube(std::shared_ptr<IShader const> shader, CMatrix4d const& transform = CMatrix4d());
	CSceneObject& AddIcosahedron(std::shared_ptr<IShader const> shader, CMatrix4d const& transform = CMatrix4d());
	CSceneObject& AddOctahedron(std::shared_ptr<IShader const> shader, CMatrix4d const& transform = CMatrix4d());


private:
	CRenderContext m_context;
	CRenderer m_renderer;
	CScene m_scene;
	std::unique_ptr<CFrameBuffer> m_pFrameBuffer;
};
