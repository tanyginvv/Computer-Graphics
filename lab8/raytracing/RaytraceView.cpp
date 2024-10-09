// 01_raytrace_baseView.cpp : implementation of the CRaytraceView class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "libgl/CFrameBuffer.h"

#include "libgl/CCheckerShader.h"
#include "libgl/CIntersection.h"
#include "COmniLightSource.h"
#include "libgl/CPlane.h"
#include "RaytraceView.h"
#include "libgl/CSceneObject.h"
#include "CSimpleDiffuseShader.h"
#include "CSphere.h"
#include "CTetrahedron.h"
#include "CCube.h"
#include "CIcosahedron.h"
#include "COctahedron.h"

CRaytraceView::CRaytraceView()
	: m_pFrameBuffer(std::make_unique<CFrameBuffer>(800, 600))
{
	/*
	Задаем цвет заднего фона сцены
	*/
	m_scene.SetBackdropColor(CVector4f(0, 0, 1, 1));

	{
		auto plane = std::make_shared<CPlane>(0, 1, 0, 1);

		CSimpleMaterial material1;
		material1.SetDiffuseColor(CVector4f(1.0f, 1.0f, 1.0f, 1.0f));
		material1.SetAmbientColor(CVector4f(0.1f, 0.1f, 0.1f, 1.0f));
		material1.SetSpecularColor(CVector4f(1.0f, 1.0f, 1.0f, 1.0f));	
		material1.SetShiness(64);

		auto plane1Shader = std::make_shared<CSimpleDiffuseShader>();
		plane1Shader->SetMaterial(material1);
		m_scene.AddObject(std::make_shared<CSceneObject>(std::move(plane), std::move(plane1Shader)));
	}

	// Создаем и добавляем в сцену сферу, имеющую заданный материал
	{
		/*
		Матрица трансформации сферы 1
		*/
		CMatrix4d sphereTransform;
		sphereTransform.Translate(0.5, 1, -2);
		auto sphere1 = std::make_shared<CSphere>(0.5); // Создаем сферу радиуса 0.5
		sphere1->SetTransform(sphereTransform);

		/*
		Материал сферы 1
		*/
		CSimpleMaterial material1;
		material1.SetDiffuseColor(CVector4f(1.0f, 0.0f, 0.0f, 1.0f));
		material1.SetAmbientColor(CVector4f(0.1f, 0.1f, 0.1f, 1.0f));
		material1.SetSpecularColor(CVector4f(1.0f, 1.0f, 1.0f, 1.0f));
		material1.SetShiness(64);

		// Шейдер сферы 1
		auto sphere1Shader = std::make_shared<CSimpleDiffuseShader>();
		sphere1Shader->SetMaterial(material1);
		m_scene.AddObject(std::make_shared<CSceneObject>(std::move(sphere1), std::move(sphere1Shader)));
	}

	// Создаем и добавляем в сцену точечный источник света
	{
		auto light = std::make_shared<COmniLightSource>(CVector3d(0, 1, 0));
		light->SetDiffuseIntensity(CVector4f(0.8f, 0.8f, 0.8f, 1));
		light->SetAmbientIntensity(CVector4f(0.1f, 0.1f, 0.1f, 1.0f));
		light->SetSpecularIntensity(CVector4f(1.0f, 1.0f, 1.0f, 1.0f));
		m_scene.AddLightSource(std::move(light));
	}

	AddSomeTetrahedrons();
	
	AddSomeCubes();

	AddSomeIcosahedrons();

	AddSomeOctahedrons();

	/*
	Задаем параметры видового порта и матрицы проецирования в контексте визуализации
	*/
	m_context.SetViewPort(CViewPort(0, 0, 800, 600));
	CMatrix4d proj;
	proj.LoadPerspective(60, 800.0 / 600.0, 0.1, 10);
	m_context.SetProjectionMatrix(proj);
	// Задаем матрицу камеры
	CMatrix4d modelView;
	modelView.LoadLookAtRH(
		0, 2.5, 7,
		0, 0, 0,
		0, 1, 0);
	m_context.SetModelViewMatrix(modelView);
}

CRaytraceView::~CRaytraceView()
{
	// Необходимо остановить фоновую работу объекта CRenderer до разрушения
	// данных класса CRaytraceView, т.к. CRenderer использует для своей работы
	// другие объекты, в частности, буфер кадра, разрушать которые можно только
	// после остановки CRenderer
	m_renderer.Stop();
}

BOOL CRaytraceView::PreTranslateMessage(MSG* pMsg)
{
	pMsg;
	return FALSE;
}

LRESULT CRaytraceView::OnEraseBkgnd(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	// Сообщаем системе, что дальнейшая очистка буфера не требуется
	return 1;
}

LRESULT CRaytraceView::OnPaint(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	CPaintDC dc(m_hWnd);

	// Получаем размеры клиентской области окна
	CRect clientRect;
	GetClientRect(clientRect);
	int clientWidth = clientRect.Width();
	int clientHeight = clientRect.Height();

	// Кисть, используемая для очистки окна
	CBrush whiteBrush;
	whiteBrush.CreateSolidBrush(0xffffff);

	// Проверка на случай отсутствия буфера кадра
	if (m_pFrameBuffer.get())
	{
		int w = m_pFrameBuffer->GetWidth();
		int h = m_pFrameBuffer->GetHeight();
		// Рисуем буфер кадра в левом верхнем углу
		// клиентской области окна
		DrawFrameBuffer(dc, 0, 0);

		// Т.к. мы отключили очистку заднего фона экрана,
		// область справа и снизу от изображения придется
		// очистить вручную

		// Очищаем правую часть клиентской области
		if (w < clientWidth)
		{
			dc.FillRect(CRect(w, 0, clientWidth, h), whiteBrush);
		}

		// Очищаем нижную часть клиентской области
		if (h < clientHeight)
		{
			dc.FillRect(CRect(0, h, clientWidth, clientHeight), whiteBrush);
		}
	}
	else // Буфер кадра по каким-то причинам не определен
	{
		ATLASSERT(!"Something bad with the frame buffer");
	}

	return 0;
}

void CRaytraceView::DrawFrameBuffer(CDC& dc, int x, int y)
{
	int w = m_pFrameBuffer->GetWidth();
	int h = m_pFrameBuffer->GetHeight();

	// Заполняем структуру BITMAPINFO (см. MSDN), чтобы
	// перекинуть содержимое буфера кадра на экран без создания дополнительных bitmap-ов
	BITMAPINFO bmpInfo;
	memset(&bmpInfo, 0, sizeof(bmpInfo));
	BITMAPINFOHEADER& hdr = bmpInfo.bmiHeader;
	hdr.biSize = sizeof(hdr);
	hdr.biWidth = w;
	// По умолчанию BMP файл хранится "вверх ногами" (сначала нижний ряд пикселей)
	// Чтобы использовать привычный вариант хранения пикселей высота должна быть отрицательной
	hdr.biHeight = -h;
	hdr.biPlanes = 1; // Количество цветовых плоскостей в изображении
	hdr.biBitCount = sizeof(std::uint32_t) << 3; // Цвет каждого пикселя занимает 32 бита
	hdr.biCompression = BI_RGB;
	hdr.biSizeImage = w * h * static_cast<int>(sizeof(std::uint32_t));

	// Используя функцию SetDIBitsToDevice (см. MSDN) перекидываем массив пикселей
	// из памяти в контекст устройства
	dc.SetDIBitsToDevice(
		x, y, // Координаты вывода внутри контекста устройства
		w, h, // Ширина и высота изображений
		0, 0, // Координаты рисуемой области изображения
		0, h, // Номер начальной строки и количество строк
		m_pFrameBuffer->GetPixels(), // Адрес пикселей
		&bmpInfo, // Адрес информации о пикселях
		DIB_RGB_COLORS); // сигнализируем о том, что значения в таблице
	// bmpInfo.bmiColors интерпретируются как RGB значения,
	// а не индексы логической палитры
}

LRESULT CRaytraceView::OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	// Запускаем процесс визуализации
	m_renderer.Render(m_scene, m_context, *m_pFrameBuffer);

	// Инициализируем таймер для отображения построенного изображения
	SetTimer(FRAMEBUFFER_UPDATE_TIMER, 500);
	return 0;
}

bool CRaytraceView::UpdateFrameBuffer()
{
	// Инициируем перерисовку окна
	RedrawWindow();

	unsigned totalChunks = 0;
	unsigned renderedChunks = 0;

	// Возвращаем true, если изображение в буфере кадра полностью построено
	return m_renderer.GetProgress(renderedChunks, totalChunks);
}

LRESULT CRaytraceView::OnTimer(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	WPARAM timerId = wParam;
	switch (timerId)
	{
	case FRAMEBUFFER_UPDATE_TIMER:
		// Если выяснилось, что изображение построено, выключаем таймер,
		// перекидывающий буфер кадра в окно
		if (UpdateFrameBuffer())
		{
			KillTimer(FRAMEBUFFER_UPDATE_TIMER);
		}
		break;
	}

	return 0;
}

// Добавляем тетраэдр
void CRaytraceView::AddSomeTetrahedrons()
{
	 CMatrix4d transform;
	 transform.Translate(3, 0.3, -1);
	 transform.Rotate(300, 0, 1, 0);
	 CSimpleMaterial material1;
	 material1.SetDiffuseColor(CVector4f(0.0f, 0.0f, 1.0f, 1.0f));
	 material1.SetAmbientColor(CVector4f(0.1f, 0.1f, 0.1f, 1.0f));
	 material1.SetSpecularColor(CVector4f(1.0f, 1.0f, 1.0f, 1.0f));
	 material1.SetShiness(64);

	 std::vector<Vertex> vertices{
		 Vertex({ -1, 0, 1 }),
		 Vertex({ +1, 0, 1 }),
		 Vertex({ 0, 0, -1 }),
		 Vertex({ 0, 2, 0 }),
	 };

	 AddTetrahedron(std::make_shared<CSimpleDiffuseShader>(material1), transform, vertices);
}

void CRaytraceView::AddSomeCubes()
{
	CMatrix4d transform;
	transform.Translate(-3, 0.3, -1);
	transform.Rotate(100, 0, 1, 0);
	CSimpleMaterial material1;
	material1.SetDiffuseColor(CVector4f(0.0f, 1.0f, 0.0f, 1.0f));
	material1.SetAmbientColor(CVector4f(0.1f, 0.1f, 0.1f, 1.0f));
	material1.SetSpecularColor(CVector4f(1.0f, 1.0f, 1.0f, 1.0f));
	material1.SetShiness(64);

	AddCube(std::make_shared<CSimpleDiffuseShader>(material1), transform);
}

void CRaytraceView::AddSomeIcosahedrons()
{
	CMatrix4d transform;
	transform.Translate(-1, 0, -5);
	CSimpleMaterial material1;
	material1.SetDiffuseColor(CVector4f(0.0f, 0.5f, 0.5f, 1.0f));
	material1.SetAmbientColor(CVector4f(0.1f, 0.1f, 0.1f, 1.0f));
	material1.SetSpecularColor(CVector4f(1.0f, 1.0f, 1.0f, 1.0f));
	material1.SetShiness(64);

	AddIcosahedron(std::make_shared<CSimpleDiffuseShader>(material1), transform);
}

void CRaytraceView::AddSomeOctahedrons()
{
	CMatrix4d transform;
	transform.Translate(4, 0, 0);
	transform.Rotate(150, 0, 1, 0);
	CSimpleMaterial material1;
	material1.SetDiffuseColor(CVector4f(0.3f, 0.2f, 0.9f, 1.0f));
	material1.SetAmbientColor(CVector4f(0.1f, 0.1f, 0.1f, 1.0f));
	material1.SetSpecularColor(CVector4f(1.0f, 1.0f, 1.0f, 1.0f));
	material1.SetShiness(64);

	AddOctahedron(std::make_shared<CSimpleDiffuseShader>(material1), transform);
}

CSceneObject& CRaytraceView::AddTetrahedron(std::shared_ptr<IShader const> shader, CMatrix4d const& transform, std::vector<Vertex> const& vertices)
{
	auto tetrahedron = std::make_shared<CTetrahedron>(transform, vertices);

	return AddSceneObject(std::move(tetrahedron), std::move(shader));
}

CSceneObject& CRaytraceView::AddCube(std::shared_ptr<IShader const> shader, CMatrix4d const& transform)
{
	auto cube = std::make_shared<CCube>(transform);

	return AddSceneObject(std::move(cube), std::move(shader));
}

CSceneObject& CRaytraceView::AddIcosahedron(std::shared_ptr<IShader const> shader, CMatrix4d const& transform)
{
	auto icosahedron = std::make_shared<CIcosahedron>(transform);

	return AddSceneObject(std::move(icosahedron), std::move(shader));
}

CSceneObject& CRaytraceView::AddOctahedron(std::shared_ptr<IShader const> shader, CMatrix4d const& transform)
{
	auto octahedron = std::make_shared<COctahedron>(transform);

	return AddSceneObject(std::move(octahedron), std::move(shader));
}

CSceneObject& CRaytraceView::AddSceneObject(std::shared_ptr<IGeometryObject const> object, std::shared_ptr<IShader const> shader)
{
	auto obj = std::make_shared<CSceneObject>(std::move(object), std::move(shader));
	m_scene.AddObject(obj);
	return *obj;
}
