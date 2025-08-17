
#include "Precompiled.h"
#include "SoftRenderer.h"
#include <random>
#include <sstream>
using namespace CK::DD;

// ���ڸ� �׸��� �Լ�
void SoftRenderer::DrawGizmo2D()
{
	auto& r = GetRenderer();
	const auto& g = Get2DGameEngine();

	// �׸��� ����
	LinearColor gridColor(LinearColor(0.8f, 0.8f, 0.8f, 0.3f));

	// ���� ���� ���
	Vector2 viewPos = g.GetMainCamera().GetTransform().GetPosition();
	Vector2 extent = Vector2(_ScreenSize.X * 0.5f, _ScreenSize.Y * 0.5f);

	// ���� �ϴܿ������� ���� �׸���
	int xGridCount = _ScreenSize.X / _Grid2DUnit;
	int yGridCount = _ScreenSize.Y / _Grid2DUnit;

	// �׸��尡 ���۵Ǵ� ���ϴ� ��ǥ �� ���
	Vector2 minPos = viewPos - extent;
	Vector2 minGridPos = Vector2(ceilf(minPos.X / (float)_Grid2DUnit), ceilf(minPos.Y / (float)_Grid2DUnit)) * (float)_Grid2DUnit;
	ScreenPoint gridBottomLeft = ScreenPoint::ToScreenCoordinate(_ScreenSize, minGridPos - viewPos);

	for (int ix = 0; ix < xGridCount; ++ix)
	{
		r.DrawFullVerticalLine(gridBottomLeft.X + ix * _Grid2DUnit, gridColor);
	}

	for (int iy = 0; iy < yGridCount; ++iy)
	{
		r.DrawFullHorizontalLine(gridBottomLeft.Y - iy * _Grid2DUnit, gridColor);
	}

	ScreenPoint worldOrigin = ScreenPoint::ToScreenCoordinate(_ScreenSize, -viewPos);
	r.DrawFullHorizontalLine(worldOrigin.Y, LinearColor::Red);
	r.DrawFullVerticalLine(worldOrigin.X, LinearColor::Green);
}

// ���� ������Ʈ ���


// ���� �� �ε��� ����ϴ� �Լ�
void SoftRenderer::LoadScene2D()
{
	// ���� �� �ε����� ����ϴ� ��� �� �ֿ� ���۷���
	auto& g = Get2DGameEngine();

}

// ���� ����(Update2D)�� ������ ����(Render2D)�� �����ϴ� ����
Vector2 currentPosition(0.f, 0.f);	// �⺻���� (0, 0)���� ����


// ���� ������ ����ϴ� �Լ� (������� �Է¿� ���� ó���� �ٷ�)
void SoftRenderer::Update2D(float InDeltaSeconds)
{
	// ���� �������� ����ϴ� ��� �� �ֿ� ���۷���
	auto& g = Get2DGameEngine();	// ���� ���� ���� ����
	// ����� �Է°��� �о���� �� �ֵ��� 
	// �Է� ������ InputManager�� ���۷����� �б� �������� ������
	const InputManager& input = g.GetInputManager();

	// ���� ������ ���� ����
	static float moveSpeed = 100.f;		// �����Ӵ� �̵� �ӵ��� ����

	// X��� Y�� �Է��� ������ �Է� ���͸� ����
	Vector2 inputValue = Vector2
	(
		input.GetAxis(InputAxis::XAxis), 
		input.GetAxis(InputAxis::YAxis)
	);

	// �̵� �ӵ�, �������� ��� �ð�(InDeltaSeconds)�� ���Ͽ� �ش� �������� �̵��� ���̸� ���
	// �̸� �Է� ���Ϳ� ���ϰ� �� ����� deltaPosition�� ����
	Vector2 deltaPosition = inputValue * moveSpeed * InDeltaSeconds;

	// ��ü�� ���� ���� ����
	currentPosition += deltaPosition;	// deltaPosition ���� �ݿ��Ͽ� ���� �������� currentPosition ���� Ȯ��
}

// ������ ������ ����ϴ� �Լ�
// ���� �������� ����� �����͸� �ݿ��� ���� ����� ����� ȭ�鿡 �����ִ� ����� ������
void SoftRenderer::Render2D()
{
	// ������ �������� ����ϴ� ��� �� �ֿ� ���۷���
	auto& r = GetRenderer();	// ȭ���� �ð��� ǥ���� ���� ������ ����� ���۷����� ������
	const auto& g = Get2DGameEngine();	// ���� ���� ����� ���۷����� �б� �������� ������

	// ��濡 ���� �׸���
	DrawGizmo2D();

	// ������ ������ ���� ����

	// ����Ÿ ���� ���� ����� ������ ���͸� ǥ��
	static float lineLength = 500.f;
	Vector2 lineStart = currentPosition * lineLength;
	Vector2 lineEnd = currentPosition * -lineLength;
	r.DrawLine(lineStart, lineEnd, LinearColor::Magenta);

	// ���͸� �Ķ��� �ȼ�(8 �ȼ�)�� ǥ��
	r.DrawPoint(currentPosition, LinearColor::Blue);
	r.DrawPoint(currentPosition + Vector2::UnitX, LinearColor::Blue);
	r.DrawPoint(currentPosition + Vector2::UnitY, LinearColor::Blue);
	r.DrawPoint(currentPosition - Vector2::UnitX, LinearColor::Blue);
	r.DrawPoint(currentPosition - Vector2::UnitY, LinearColor::Blue);
	r.DrawPoint(currentPosition + Vector2::One, LinearColor::Blue);
	r.DrawPoint(currentPosition - Vector2::One, LinearColor::Blue);
	r.DrawPoint(currentPosition + Vector2(1.f, -1.f), LinearColor::Blue);
	r.DrawPoint(currentPosition - Vector2(1.f, -1.f), LinearColor::Blue);

	// ������ ���� ��ǥ�� ���ܿ� ǥ��
	r.PushStatisticText("Coordination : " + currentPosition.ToString());
}

// �޽ø� �׸��� �Լ�
void SoftRenderer::DrawMesh2D(const class DD::Mesh& InMesh, const Matrix3x3& InMatrix, const LinearColor& InColor)
{
}

// �ﰢ���� �׸��� �Լ�
void SoftRenderer::DrawTriangle2D(std::vector<DD::Vertex2D>& InVertices, const LinearColor& InColor, FillMode InFillMode)
{
}
