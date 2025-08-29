
#include "Precompiled.h"
#include "SoftRenderer.h"
#include <random>
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

// 게임 로직과 렌더링 로직이 공유하는 변수
Vector2 currentPosition;
float currentScale = 10.f;	// 기본 값을 10으로 지정

// 게임 로직을 담당하는 함수
void SoftRenderer::Update2D(float InDeltaSeconds)
{
	// 게임 로직에서 사용하는 모듈 내 주요 레퍼런스
	auto& g = Get2DGameEngine();
	const InputManager& input = g.GetInputManager();

	// 게임 로직의 로컬 변수
	static float moveSpeed = 100.f;
	static float scaleMin = 5.f;	// 최소 크기 값
	static float scaleMax = 20.f;	// 최대 크기 값
	static float scaleSpeed = 20.f;	// 입력에 따른 크기 변화 속도

	Vector2 inputVector = Vector2
	(
		input.GetAxis(InputAxis::XAxis), 
		input.GetAxis(InputAxis::YAxis)
	).GetNormalize();

	Vector2 deltaPosition = inputVector * moveSpeed * InDeltaSeconds;
	float deltaScale = input.GetAxis(InputAxis::ZAxis) * scaleSpeed * InDeltaSeconds;	// 현재 프레임에서 Z축 입력에 따른 크기의 변화량을 계산

	// 물체의 최종 상태 설정
	currentPosition += deltaPosition;
	// 최종 크기ㅣ 값에 변화량을 반영하되 Clamp 함수를 사용해 
	// 지정한 최대/최소값 사이를 넘지 못하도록 함
	currentScale = Math::Clamp(currentScale + deltaScale, scaleMin, scaleMax);	
}

// 렌더링 로직을 담당하는 함수
void SoftRenderer::Render2D()
{
	// 렌더링 로직에서 사용하는 모듈 내 주요 레퍼런스
	auto& r = GetRenderer();
	const auto& g = Get2DGameEngine();

	// 배경에 격자 그리기
	DrawGizmo2D();

	// 렌더링 로직의 로컬 변수
	float rad = 0.f;	// 현재 각을 지정하는 변수
	static float increment = 0.001f;	// 각을 서서히 증가시키도록 증분값 변수 선언
	static std::vector<Vector2> hearts;	// 하트를 구성하는 점을 모아둘 컨테이너

	// 하트를 구성하는 점 생성
	if (hearts.empty())
	{
		for (rad = 0.f; rad < Math::TwoPI; rad += increment)	// 각을 0부터 2파이까지 서서히 증가시키는 루프 실행
		{
			// 하트 방정식을 사용해 해당 각의 x와 y를 계산
			float sin = sinf(rad);
			float cos = cosf(rad);
			float cos2 = cosf(rad * 2);
			float cos3 = cosf(rad * 3);
			float cos4 = cosf(rad * 4);
			float x = 16.f * sin * sin * sin;
			float y = 13.f * cos - 5.f * cos2 - 2.f * cos3 - cos4;

			hearts.push_back(Vector2(x, y));	// x와 y값으로 벡터를 만든 후 이를 hearts에 추가
		}
	}

	for (auto const& v : hearts)
	{
		r.DrawPoint(v * currentScale + currentPosition, LinearColor::Blue);
	}

	// 현재 위치와 스케일을 화면에 출력
	r.PushStatisticText(std::string("Position : ") + currentPosition.ToString());
	r.PushStatisticText(std::string("Scale : ") + std::to_string(currentScale));
}

// 메시를 그리는 함수
void SoftRenderer::DrawMesh2D(const class DD::Mesh& InMesh, const Matrix3x3& InMatrix, const LinearColor& InColor)
{
}

// 삼각형을 그리는 함수
void SoftRenderer::DrawTriangle2D(std::vector<DD::Vertex2D>& InVertices, const LinearColor& InColor, FillMode InFillMode)
{
}
