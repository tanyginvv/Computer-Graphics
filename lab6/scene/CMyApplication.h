#pragma once
#include "GLApplication.h"
#include "CModelRenderer.h"

class CMyApplication : public CGLApplication
{
public:
	CMyApplication(const char* title, int width, int height);
	~CMyApplication(void) = default;

protected:
	virtual void OnInit();
	virtual void OnDisplay(void);
	virtual void OnReshape(int width, int height);
	virtual void OnMouse(int button, int state, int x, int y);
	virtual void OnMotion(int x, int y);
	virtual void OnKeyboard(unsigned char key, int x, int y);
	

private:
	// ���������� ���������������� ������� �������������-����
	static void NormalizeModelViewMatrix(void);

	void DrawWhiteTeam() const;
	void DrawBlackTeam() const;

	void ShowFirstMove() const;
	void ShowSecondMove() const;
	void ShowThirdMove() const;
	void ShowFourthMove() const;
	void ShowAnimation() const;

	int m_windowWidth;
	int m_windowHeight;
	CModel m_board;
	CModel m_rook;
	CModel m_pawn;
	CModel m_bishop;
	CModel m_knight;
	CModel m_king;
	CModel m_queen;
	CModel m_emptyModel;
	CModelRenderer m_renderer;

	// ������� ������ ������ ������ �������� �� �������� ����
	static void RotateCamera(GLfloat rotateX, GLfloat rotateY);
	// ����, ����������������� � ��������� ����� ������ ����
	bool m_leftButtonPressed;

	// ������ ���������� ������� ����
	int m_mouseX;
	int m_mouseY;
};
