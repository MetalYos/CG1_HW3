#include "Camera.h"

Camera::Camera() : isPerspective(false)
{
}


Camera::~Camera()
{
}

void Camera::Translate(Mat4& T)
{
	cTransform = cTransform * T;
}

void Camera::Scale(Mat4& S, bool aroundEye)
{
	if (aroundEye)
	{
		Mat4 T = Mat4::Translate(camParams.EyeCam) * S * Mat4::Translate(-camParams.EyeCam);
		cTransform = cTransform * T;
	}
	else
		cTransform = cTransform * S;
}

void Camera::Rotate(Mat4& R, bool aroundEye)
{
	if (aroundEye)
	{
		Mat4 T = Mat4::Translate(camParams.EyeCam) * R * Mat4::Translate(-camParams.EyeCam);
		cTransform = cTransform * T;
	}
	else
		cTransform = cTransform * R;
}

Mat4 Camera::GetTranform() const
{
	return cTransform;
}

void Camera::SetOrthographic(double left, double right, double top, double bottom, double z_near, double z_far)
{
	Mat4 result;
	result[0][0] = -2.0 / (right - left);
	result[1][1] = -2.0 / (top - bottom);
	result[2][2] = 2.0 / (z_far - z_near);
	result[3][0] = -(right + left) / (right - left);
	result[3][1] = -(top + bottom) / (top - bottom);
	result[3][2] = -(z_far + z_near) / (z_far - z_near);
	result[3][3] = 1.0;

	projection = result;
	orthographic = result;
	isPerspective = false;

	orthographicParams.Left = left;
	orthographicParams.Right = right;
	orthographicParams.Top = top;
	orthographicParams.Bottom = bottom;
	orthographicParams.Near = z_near;
	orthographicParams.Far = z_far;
}

void Camera::SetOrthographic(double height, double aspectR, double z_near, double z_far)
{
	double width = height * aspectR;
	SetOrthographic(-width / 2.0, width / 2.0, height / 2.0, -height / 2.0, 1, 1000.0);
}

void Camera::SetPerspective(double left, double right, double top, double bottom, double z_near, double z_far)
{
	Mat4 result;
	result[0][0] = 2 * z_near / (right - left);
	result[0][2] = (right + left) / (right - left);
	result[1][1] = 2 * z_near / (top - bottom);
	result[1][2] = (top + bottom) / (top - bottom);
	result[2][2] = -(z_far + z_near) / (z_far - z_near);
	result[2][3] = -2 * z_far * z_near / (z_far - z_near);
	result[3][2] = -1.0;
	result[3][3] = 0.0;
	result.Transpose();

	projection = result;
	perspective = result;
	isPerspective = true;

	perspectiveParams.Left = left;
	perspectiveParams.Right = right;
	perspectiveParams.Top = top;
	perspectiveParams.Bottom = bottom;
	perspectiveParams.Near = z_near;
	perspectiveParams.Far = z_far;
	perspectiveParams.FOV = ToDegrees(atan(top / z_near)) * 2.0;
}

void Camera::SetPerspective(double fovy, double aspectR, double z_near, double z_far)
{
	double f = tan(ToRadians(fovy / 2.0));
	double top = z_near * f;
	double right = top * aspectR;

	SetPerspective(-right, right, top, -top, z_near, z_far);

	perspectiveParams.FOV = fovy;
	perspectiveParams.AspectRatio = aspectR;
}

void Camera::SetPerspective2(double fovy, double aspectR, double alpha, double d)
{
	double f = tan(ToRadians(fovy / 2.0));

	// Set the perspective warp matrix as seen in lecture (added support for FOV and aspect ratio)
	Mat4 result;
	result[0][0] = 1.0 / (f * aspectR);
	result[1][1] = 1.0 / f;
	result[2][2] = -d / (d - alpha);
	result[2][3] = 1.0 / d;
	result[3][2] = -alpha * d / (d - alpha);
	result[3][3] = 0.0;

	// Moves the box so it's center in z axis will be at 0 
	//result = result * Mat4::Translate(0.0, 0.0, -d / 2.0);
	// Scales the box so it's size in z axis is 2.0
	//result = result * Mat4::Scale(1.0, 1.0, 2.0 / d);

	projection = result;
	perspective = result;
	isPerspective = true;

	perspectiveParams.Near = alpha;
	perspectiveParams.Far = d;
	perspectiveParams.FOV = fovy;
	perspectiveParams.AspectRatio = aspectR;
}

Mat4 Camera::GetProjection() const
{
	return projection;
}

const CameraParameters & Camera::GetCameraParameters() const
{
	return camParams;
}

const PerspectiveParams & Camera::GetPerspectiveParameters() const
{
	return perspectiveParams;
}

const OrthographicParams & Camera::GetOrthographicParameters() const
{
	return orthographicParams;
}

void Camera::SwitchProjection(bool isPerspective)
{
	if (this->isPerspective != isPerspective)
	{
		projection = isPerspective ? perspective : orthographic;
		this->isPerspective = isPerspective;
	}
}

bool Camera::IsPerspective() const
{
	return isPerspective;
}

void Camera::LookAt(const Vec4 & eye, const Vec4 & at, const Vec4 & up)
{
	Vec4 n = Vec4::Normalize3(eye - at);
	n[3] = 0;
	Vec4 u = Vec4::Normalize3(Vec4::Cross(up, n));
	u[3] = 0;
	Vec4 v = Vec4::Normalize3(Vec4::Cross(n, u));
	v[3] = 0;
	Mat4 c(u, v, n, Vec4(0.0, 0.0, 0.0, 1.0));
	c.Transpose();
	cTransform = Mat4::Translate(-eye) * c;
	cTransform[3][3] = 1.0;

	camParams.Eye = eye;
	camParams.EyeCam = eye * cTransform;
	camParams.Front = n;
	camParams.Side = u;
	camParams.Up = v;
}
