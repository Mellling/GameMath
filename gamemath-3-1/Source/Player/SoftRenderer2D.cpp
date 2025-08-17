
#include "Precompiled.h"
#include "SoftRenderer.h"
#include <random>
#include <sstream>
using namespace CK::DD;

// 격자를 그리는 함수
void SoftRenderer::DrawGizmo2D()
{
	auto& r = GetRenderer();
	const auto& g = Get2DGameEngine();

	// 그리드 색상
	LinearColor gridColor(LinearColor(0.8f, 0.8f, 0.8f, 0.3f));

	// 뷰의 영역 계산
	Vector2 viewPos = g.GetMainCamera().GetTransform().GetPosition();
	Vector2 extent = Vector2(_ScreenSize.X * 0.5f, _ScreenSize.Y * 0.5f);

	// 좌측 하단에서부터 격자 그리기
	int xGridCount = _ScreenSize.X / _Grid2DUnit;
	int yGridCount = _ScreenSize.Y / _Grid2DUnit;

	// 그리드가 시작되는 좌하단 좌표 값 계산
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

// 게임 오브젝트 목록


// 최초 씬 로딩을 담당하는 함수
void SoftRenderer::LoadScene2D()
{
	// 최초 씬 로딩에서 사용하는 모듈 내 주요 레퍼런스
	auto& g = Get2DGameEngine();

}

// 게임 로직(Update2D)과 렌더링 로직(Render2D)이 공유하는 변수
Vector2 currentPosition(0.f, 0.f);	// 기본값을 (0, 0)으로 지정


// 게임 로직을 담당하는 함수 (사용자의 입력에 따른 처리를 다룸)
void SoftRenderer::Update2D(float InDeltaSeconds)
{
	// 게임 로직에서 사용하는 모듈 내 주요 레퍼런스
	auto& g = Get2DGameEngine();	// 개암 앤잔 모듈애 접근
	// 사용자 입력값을 읽어들일 수 있도록 
	// 입력 관리자 InputManager의 레퍼런스를 읽기 전용으로 가져옴
	const InputManager& input = g.GetInputManager();

	// 게임 로직의 로컬 변수
	static float moveSpeed = 100.f;		// 프레임당 이동 속도를 지정

	// X축과 Y축 입력을 결합해 입력 벡터를 지정
	Vector2 inputValue = Vector2
	(
		input.GetAxis(InputAxis::XAxis), 
		input.GetAxis(InputAxis::YAxis)
	);

	// 이동 속도, 프레임의 경과 시간(InDeltaSeconds)을 곱하여 해당 프레임이 이동할 길이를 계산
	// 이를 입력 벡터에 곱하고 그 결과를 deltaPosition애 저장
	Vector2 deltaPosition = inputValue * moveSpeed * InDeltaSeconds;

	// 물체의 최종 상태 결정
	currentPosition += deltaPosition;	// deltaPosition 값을 반영하여 현재 프레임의 currentPosition 값을 확정
}

// 렌더링 로직을 담당하는 함수
// 게임 로직에서 계산한 데이터를 반영해 최종 결과를 모니터 화면에 보여주는 기능을 구현함
void SoftRenderer::Render2D()
{
	// 렌더링 로직에서 사용하는 모듈 내 주요 레퍼런스
	auto& r = GetRenderer();	// 화면의 시각적 표현을 위해 렌더러 모듈의 레퍼런스를 가져옴
	const auto& g = Get2DGameEngine();	// 게임 엔진 모듈의 레퍼런스를 읽기 전용으로 가져옴

	// 배경에 격자 그리기
	DrawGizmo2D();

	// 렌더링 로직의 로컬 변수

	// 마젠타 색의 선을 사용해 평행한 백터를 표현
	static float lineLength = 500.f;
	Vector2 lineStart = currentPosition * lineLength;
	Vector2 lineEnd = currentPosition * -lineLength;
	r.DrawLine(lineStart, lineEnd, LinearColor::Magenta);

	// 백터를 파란색 픽셀(8 픽셀)로 표현
	r.DrawPoint(currentPosition, LinearColor::Blue);
	r.DrawPoint(currentPosition + Vector2::UnitX, LinearColor::Blue);
	r.DrawPoint(currentPosition + Vector2::UnitY, LinearColor::Blue);
	r.DrawPoint(currentPosition - Vector2::UnitX, LinearColor::Blue);
	r.DrawPoint(currentPosition - Vector2::UnitY, LinearColor::Blue);
	r.DrawPoint(currentPosition + Vector2::One, LinearColor::Blue);
	r.DrawPoint(currentPosition - Vector2::One, LinearColor::Blue);
	r.DrawPoint(currentPosition + Vector2(1.f, -1.f), LinearColor::Blue);
	r.DrawPoint(currentPosition - Vector2(1.f, -1.f), LinearColor::Blue);

	// 백터의 현재 좌표를 우상단에 표시
	r.PushStatisticText("Coordination : " + currentPosition.ToString());
}

// 메시를 그리는 함수
void SoftRenderer::DrawMesh2D(const class DD::Mesh& InMesh, const Matrix3x3& InMatrix, const LinearColor& InColor)
{
}

// 삼각형을 그리는 함수
void SoftRenderer::DrawTriangle2D(std::vector<DD::Vertex2D>& InVertices, const LinearColor& InColor, FillMode InFillMode)
{
}
