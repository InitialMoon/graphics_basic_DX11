#include "GameApp.h"
#include "d3dUtil.h"
#include "DXTrace.h"
using namespace DirectX;

GameApp::GameApp(HINSTANCE hInstance)
	: D3DApp(hInstance), 
	m_IndexCount(),
	m_VSConstantBuffer(),
	m_PSConstantBuffer(),
	m_DirLight(),
	m_PointLight(),
	m_SpotLight(),
	mirrorPosX(200),
	mirrorPosZ(100),
	viewPos(100.0f, 100.0f, -60.0f),
	viewRot(0.7f, 0.0f, 0.0f),
	realMirrorPos(270.7f)
{
}

GameApp::~GameApp()
{
}

bool GameApp::Init()
{
	if (!D3DApp::Init())
		return false;

	if (!InitEffect())
		return false;

	if (!InitResource())
		return false;

	// 初始化鼠标，键盘不需要
	m_pMouse->SetWindow(m_hMainWnd);
	m_pMouse->SetMode(DirectX::Mouse::MODE_ABSOLUTE);

	return true;
}

void GameApp::OnResize()
{
	D3DApp::OnResize();
}

void GameApp::UpdateScene(float dt)
{
	angle += 1.0f * dt;
	static float phi = 0.1f, theta = 0.1f;
	phi += 0.0001f, theta += 0.00015f;
	XMMATRIX W = XMMatrixRotationX(phi) * XMMatrixRotationY(theta);
	m_VSConstantBuffer.world = XMMatrixTranspose(W);
	m_VSConstantBuffer.worldInvTranspose = XMMatrixInverse(nullptr, W);	// 两次转置可以抵消

	// 获取鼠标状态
	Mouse::State mouseState = m_pMouse->GetState();
	Mouse::State lastMouseState = m_MouseTracker.GetLastState();
	// 获取键盘状态
	Keyboard::State keyState = m_pKeyboard->GetState();
	Keyboard::State lastKeyState = m_KeyboardTracker.GetLastState();

	// 更新鼠标按钮状态跟踪器，仅当鼠标按住的情况下才进行移动
	m_MouseTracker.Update(mouseState);
	m_KeyboardTracker.Update(keyState);

	XMFLOAT3 pos = { 0, 0, 0 };
	float moveSpeed = 40.0f;
	if (keyState.IsKeyDown(Keyboard::LeftShift)) moveSpeed *= 2.0f;
	if (keyState.IsKeyDown(Keyboard::W)) pos.z += moveSpeed * dt;
	if (keyState.IsKeyDown(Keyboard::S)) pos.z -= moveSpeed * dt;
	if (keyState.IsKeyDown(Keyboard::A)) pos.x -= moveSpeed * dt;
	if (keyState.IsKeyDown(Keyboard::D)) pos.x += moveSpeed * dt;
	if (keyState.IsKeyDown(Keyboard::Q)) viewRot.z += 2.0f * dt;
	if (keyState.IsKeyDown(Keyboard::E)) viewRot.z -= 2.0f * dt;

	if (mouseState.leftButton == true && m_MouseTracker.leftButton == m_MouseTracker.HELD)
	{
		viewRot.y += (mouseState.x - lastMouseState.x) * 1.5f * dt;
		viewRot.x += (mouseState.y - lastMouseState.y) * 1.5f * dt;
	}

	// 转角限制
	if (viewRot.x > 1.5f) viewRot.x = 1.5f;
	else if (viewRot.x < -1.5f) viewRot.x = -1.5f;
	if (viewRot.z > 1.5f) viewRot.z = 1.5f;
	else if (viewRot.z < -1.5f) viewRot.z = -1.5f;

	// 汇总所有变化矩阵
	XMStoreFloat3(&pos, XMVector3Transform(XMLoadFloat3(&pos), XMMatrixRotationX(viewRot.x) * XMMatrixRotationZ(viewRot.z) * XMMatrixRotationY(viewRot.y)));
	viewPos.x += pos.x;
	viewPos.y += pos.y;
	viewPos.z += pos.z;
	m_VSConstantBuffer.view = XMMatrixTranspose(XMMatrixTranslation(-viewPos.x, -viewPos.y, -viewPos.z) * XMMatrixRotationY(-viewRot.y) * XMMatrixRotationZ(-viewRot.z) * XMMatrixRotationX(-viewRot.x));

	// 新增部分
	// lightType 用于控制三盏灯的状态
	if (m_KeyboardTracker.IsKeyPressed(Keyboard::D1))
	{
		m_PSConstantBuffer.dirLight = m_DirLight;
		m_PSConstantBuffer.pointLight0 = PointLight();
		m_PSConstantBuffer.pointLight1 = PointLight();
		m_PSConstantBuffer.pointLight2 = PointLight();
		m_PSConstantBuffer.spotLight = SpotLight();
		lightType = 1;
	}
	else if (m_KeyboardTracker.IsKeyPressed(Keyboard::D2))
	{
		m_PSConstantBuffer.dirLight = DirectionalLight();
		m_PSConstantBuffer.pointLight0 = m_PointLight[0];
		m_PSConstantBuffer.pointLight1 = m_PointLight[1];
		m_PSConstantBuffer.pointLight2 = m_PointLight[2];
		m_PSConstantBuffer.spotLight = SpotLight();
		lightType = 2;
	}
	else if (m_KeyboardTracker.IsKeyPressed(Keyboard::D3))
	{
		m_PSConstantBuffer.dirLight = DirectionalLight();
		m_PSConstantBuffer.pointLight0 = PointLight();
		m_PSConstantBuffer.pointLight1 = PointLight();
		m_PSConstantBuffer.pointLight2 = PointLight();
		m_PSConstantBuffer.spotLight = m_SpotLight;
		lightType = 3;
	}

	// 萤火虫
	if (lightType == 2) {
		// 萤火虫在做圆周运动的同时，还在上下穿梭，产生忽大忽小的效果
		m_PointLight[1].position.x = 50 * sin(angle + (2 / 3) * 3.14) + 100;
		m_PointLight[1].position.z = 50 * cos(angle + (2 / 3) * 3.14) + 100;
		m_PointLight[1].position.y = 50 * cos(angle);

		m_PointLight[2].position.x = 30 * sin(-angle / 2) + 50;
		m_PointLight[2].position.z = 100* cos(-angle / 2) + 100;
		m_PointLight[2].position.y = 50 * cos(angle);

		m_PSConstantBuffer.pointLight1.position = m_PointLight[1].position;
		m_PSConstantBuffer.pointLight2.position = m_PointLight[2].position;
	}

	// 探照灯
	if (lightType == 3) {
		m_SpotLight.position = viewPos;
		m_PSConstantBuffer.spotLight.position = m_SpotLight.position;
		DirectX::XMFLOAT3 initialLookTo = DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f);
		XMStoreFloat3(&m_SpotLight.direction, XMVector3Transform(XMLoadFloat3(&initialLookTo), XMMatrixRotationX(viewRot.x) * XMMatrixRotationZ(viewRot.z) * XMMatrixRotationY(viewRot.y)));
		m_PSConstantBuffer.spotLight.direction = m_SpotLight.direction;

		//高光跟随
		m_PSConstantBuffer.eyePos.x = viewPos.x;
		m_PSConstantBuffer.eyePos.y = viewPos.y;
		m_PSConstantBuffer.eyePos.z = viewPos.z;
	}


	// 更新常量缓冲区，让立方体转起来
	D3D11_MAPPED_SUBRESOURCE mappedData;
	HR(m_pd3dImmediateContext->Map(m_pConstantBuffers[0].Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedData));
	memcpy_s(mappedData.pData, sizeof(VSConstantBuffer), &m_VSConstantBuffer, sizeof(VSConstantBuffer));
	m_pd3dImmediateContext->Unmap(m_pConstantBuffers[0].Get(), 0);

}

// 镜子的绘制
bool GameApp::DrawMirror() {
	D3D11_MAPPED_SUBRESOURCE mappedData;
	Geometry::MeshData<VertexPosNormalTex, WORD> md_mirror = Geometry::CreatePlane(XMFLOAT3(), XMFLOAT2(200.0f, 100.0f), XMFLOAT2(1.0f, 1.0f));

	m_pd3dImmediateContext->IASetInputLayout(m_pVertexLayoutTex.Get());
	ResetMesh(md_mirror);
	m_pd3dImmediateContext->VSSetShader(m_pVertexShaderTex.Get(), nullptr, 0);
	m_pd3dImmediateContext->PSSetShader(m_pPixelShaderTex.Get(), nullptr, 0);
	m_pd3dImmediateContext->GSSetShader(nullptr, nullptr, 0);
	m_pd3dImmediateContext->PSSetShaderResources(0, 1, m_pIceCrate.GetAddressOf());

	// 更新VS常量缓冲区资源 
	DirectX::XMMATRIX mScale = XMMatrixScaling(4, 4, 1);// 缩放矩阵
	m_VSConstantBuffer.world = XMMatrixTranspose(XMMatrixRotationX(-XM_PIDIV2) * XMMatrixTranslation(60.0f, 0.0f, realMirrorPos) * mScale);
	m_VSConstantBuffer.worldInvTranspose = XMMatrixInverse(nullptr, XMMatrixTranspose(m_VSConstantBuffer.world));

	HR(m_pd3dImmediateContext->Map(m_pConstantBuffers[0].Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedData));
	memcpy_s(mappedData.pData, sizeof(VSConstantBuffer), &m_VSConstantBuffer, sizeof(VSConstantBuffer));
	m_pd3dImmediateContext->Unmap(m_pConstantBuffers[0].Get(), 0);

	m_PSConstantBuffer.material.ambient = XMFLOAT4(0.8f, 0.8f, 0.8f, 1.0f);
	m_PSConstantBuffer.material.diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 0.7f);
	m_PSConstantBuffer.material.specular = XMFLOAT4(0.4f, 0.4f, 0.4f, 35.0f);

	// 更新PS常量缓冲区资源
	HR(m_pd3dImmediateContext->Map(m_pConstantBuffers[1].Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedData));
	memcpy_s(mappedData.pData, sizeof(PSConstantBuffer), &m_PSConstantBuffer, sizeof(PSConstantBuffer));
	m_pd3dImmediateContext->Unmap(m_pConstantBuffers[1].Get(), 0);
	m_pd3dImmediateContext->DrawIndexed(m_IndexCount, 0, 0);

	return true;
}

// 纹理板
bool GameApp::DrawBoard() {
	auto meshData1 = Geometry::CreateBox<VertexPosNormalTex>(0.5f, 300.0f, 300.0f, XMFLOAT4(0.0f, 1.0f, 1.0f, 1.0f));
	XMFLOAT4 texCord[8];
	// 右面(+X面)
	texCord[0] = XMFLOAT4(0.0f, 1.0f, 0.0f, 0.0f);
	texCord[1] = XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);
	texCord[2] = XMFLOAT4(1.0f, 0.0f, 0.0f, 0.0f);
	texCord[3] = XMFLOAT4(1.0f, 1.0f, 0.0f, 0.0f);
	// 左面(-X面)
	texCord[4] = XMFLOAT4(1.0f, 1.0f, 0.0f, 0.0f);
	texCord[5] = XMFLOAT4(1.0f, 0.0f, 0.0f, 0.0f);
	texCord[6] = XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);
	texCord[7] = XMFLOAT4(0.0f, 1.0f, 0.0f, 0.0f);

	for (int i = 0; i < 8; i++) {
		texCord[i].x -= 0.5;
		texCord[i].y -= 0.5;
		XMStoreFloat4(&texCord[i], XMVector4Transform(XMLoadFloat4(&texCord[i]), XMMatrixRotationZ(0.5 * angle)));
		texCord[i].x += 0.5;
		texCord[i].y += 0.5;
	}
	for (int i = 0; i < 8; i++) {
		meshData1.vertexVec[i].tex.x = texCord[i].x;
		meshData1.vertexVec[i].tex.y = texCord[i].y;
	}

	for (int i = 8; i < 24; i++) {
		meshData1.vertexVec[i].tex = XMFLOAT2(0.0f, 0.0f);
	}

	ResetMesh(meshData1);

	m_pd3dImmediateContext->IASetInputLayout(m_pVertexLayoutTex.Get());
	m_pd3dImmediateContext->VSSetShader(m_pVertexShaderTex.Get(), nullptr, 0);
	m_pd3dImmediateContext->PSSetShader(m_pPixelShaderTex.Get(), nullptr, 0);
	m_pd3dImmediateContext->GSSetShader(nullptr, nullptr, 0);
	m_pd3dImmediateContext->PSSetShaderResources(0, 1, m_pWoodCrate.GetAddressOf());

	// 更新VS常量缓冲区资源
	m_VSConstantBuffer.world = XMMatrixTranspose(XMMatrixTranslation(mirrorPosX, 0, mirrorPosZ));
	m_VSConstantBuffer.worldInvTranspose = XMMatrixInverse(nullptr, XMMatrixTranspose(m_VSConstantBuffer.world));
	D3D11_MAPPED_SUBRESOURCE mappedData;
	HR(m_pd3dImmediateContext->Map(m_pConstantBuffers[0].Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedData));
	memcpy_s(mappedData.pData, sizeof(VSConstantBuffer), &m_VSConstantBuffer, sizeof(VSConstantBuffer));
	m_pd3dImmediateContext->Unmap(m_pConstantBuffers[0].Get(), 0);

	m_PSConstantBuffer.material.ambient = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	m_PSConstantBuffer.material.diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 0.5f);
	m_PSConstantBuffer.material.specular = XMFLOAT4(1.0f, 1.0f, 1.0f, 5.0f);
	// 更新PS常量缓冲区资源
	HR(m_pd3dImmediateContext->Map(m_pConstantBuffers[1].Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedData));
	memcpy_s(mappedData.pData, sizeof(PSConstantBuffer), &m_PSConstantBuffer, sizeof(PSConstantBuffer));
	m_pd3dImmediateContext->Unmap(m_pConstantBuffers[1].Get(), 0);

	m_pd3dImmediateContext->DrawIndexed(m_IndexCount, 0, 0);
	return true;
}

// 字符森林
bool GameApp::DrawName() {
	//无纹理
	m_pd3dImmediateContext->IASetInputLayout(m_pVertexLayout.Get());
	auto meshData = Geometry::CreateName<VertexPosNormalTex>();
	ResetMesh(meshData);
	m_pd3dImmediateContext->VSSetShader(m_pVertexShader.Get(), nullptr, 0);
	m_pd3dImmediateContext->PSSetShader(m_pPixelShader.Get(), nullptr, 0);
	m_pd3dImmediateContext->GSSetShader(m_pGeometryShader.Get(), nullptr, 0);

	DirectX::XMMATRIX Ptranslate = XMMatrixTranslation((angle * 1.5 - 8.6f) * 3.5f, 2.0f, (angle * 1.5 - 6.5f) * 4.5f);
	// 绘制几何模型
	int WIDTH = 15;
	int HEIGH = 18;
	for (int i = 1; i <= WIDTH; i++)
	{
		for (int j = 1; j <= HEIGH; j++)
		{
			// 旋转以及缩放方式的改变
			float scale = (sinf(angle * 1.0f + (WIDTH - i) * 0.2f + 5.0f) * cosf(angle * 1.0f + j * 0.2f) * 2 + 1.3f) * 0.3f; // scale 是对顶点的整体的缩放比例

			DirectX::XMMATRIX mScale = XMMatrixScaling(scale, scale, scale);// 缩放矩阵
			DirectX::XMMATRIX mRotate = XMMatrixRotationX(1.9f) * XMMatrixRotationY(2 * sinf(angle)) * XMMatrixRotationZ(1.0f);// 旋转矩阵
			DirectX::XMMATRIX mTranslate = XMMatrixTranslation((i * 1.5 - 8.6f) * 3.5f, 2.0f, (j * 1.5 - 6.5f) * 4.5f);// 平移矩阵

			// 下面的操作就是在CPU这一端将三个变换矩阵进行连成的操作，三次变换放在一个矩阵中，传给GPU，减少传递的参数
			m_VSConstantBuffer.world = XMMatrixTranspose(mScale * mRotate * mTranslate); // mul(vec, mat) 中为行向量，矩阵右乘，顺序SRT, 参考https://www.cnblogs.com/X-Jun/p/9808727.html#_lab2_1_1
			m_VSConstantBuffer.worldInvTranspose = XMMatrixInverse(nullptr, XMMatrixTranspose(m_VSConstantBuffer.world));	// 两次转置可以抵消，这里太坑了

			// 更新常量缓冲区，让立方体转起来
			D3D11_MAPPED_SUBRESOURCE mappedData;
			HR(m_pd3dImmediateContext->Map(m_pConstantBuffers[0].Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedData));
			memcpy_s(mappedData.pData, sizeof(m_VSConstantBuffer), &m_VSConstantBuffer, sizeof(m_VSConstantBuffer));
			m_pd3dImmediateContext->Unmap(m_pConstantBuffers[0].Get(), 0);
			m_pd3dImmediateContext->DrawIndexed(144, 0, 0);

			// 围绕大字符的小字符
			srand(i * j + 1);
			DirectX::XMMATRIX mScaleChild = XMMatrixScaling(0.3f, 0.3f, 0.3f);
			DirectX::XMMATRIX mTranslateChild = XMMatrixTranslation(cosf(angle) , sinf(angle) * 10.0f, 4.0f);
			for (int k = 1; k <= 1 + rand() % 5; k++)
			{
				DirectX::XMMATRIX mRotateChild = XMMatrixRotationX(rand() + angle) * XMMatrixRotationY(rand() + angle) * XMMatrixRotationZ(rand() + angle);
				m_VSConstantBuffer.world = XMMatrixTranspose(mScaleChild * mTranslateChild * mScale * mRotateChild * mTranslate);
				m_VSConstantBuffer.worldInvTranspose = XMMatrixInverse(nullptr, XMMatrixTranspose(m_VSConstantBuffer.world));	// 两次转置可以抵消

				D3D11_MAPPED_SUBRESOURCE mappedData;
				HR(m_pd3dImmediateContext->Map(m_pConstantBuffers[0].Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedData));
				memcpy_s(mappedData.pData, sizeof(m_VSConstantBuffer), &m_VSConstantBuffer, sizeof(m_VSConstantBuffer));
				m_pd3dImmediateContext->Unmap(m_pConstantBuffers[0].Get(), 0);
				m_pd3dImmediateContext->DrawIndexed(144, 0, 0);
			}
		}
	}
	return true;
}

void GameApp::DrawScene()
{
	assert(m_pd3dImmediateContext);
	assert(m_pSwapChain);


	m_pd3dImmediateContext->ClearRenderTargetView(m_pRenderTargetView.Get(), reinterpret_cast<const float*>(&Colors::Black));
	m_pd3dImmediateContext->ClearDepthStencilView(m_pDepthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	
	// 7新增部分
	// 反射部分先绘制
	m_pd3dImmediateContext->RSSetState(nullptr);
	m_pd3dImmediateContext->OMSetDepthStencilState(RenderStates::DSSWriteStencil.Get(), 1);
	m_pd3dImmediateContext->OMSetBlendState(RenderStates::BSNoColorWrite.Get(), nullptr, 0xFFFFFFFF);

	DrawMirror();

	// 开启反射参数，告诉着色器进行反射操作
	m_VSConstantBuffer.isReflection = true;
	D3D11_MAPPED_SUBRESOURCE mappedData0;
	HR(m_pd3dImmediateContext->Map(m_pConstantBuffers[0].Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedData0));
	memcpy_s(mappedData0.pData, sizeof(VSConstantBuffer), &m_VSConstantBuffer, sizeof(VSConstantBuffer));
	m_pd3dImmediateContext->Unmap(m_pConstantBuffers[0].Get(), 0);

	// 绘制不透明物体，需要顺时针裁剪
	// 仅对模板值为1的镜面区域绘制
	m_pd3dImmediateContext->RSSetState(RenderStates::RSCullClockWise.Get());
	m_pd3dImmediateContext->OMSetDepthStencilState(RenderStates::DSSDrawWithStencil.Get(), 1);
	m_pd3dImmediateContext->OMSetBlendState(nullptr, nullptr, 0xFFFFFFFF);

	DrawName();

	// ******************
	// 3. 绘制透明的反射物体
	// 关闭顺逆时针裁剪
	// 仅对模板值为1的镜面区域绘制
	// 透明混合
	m_pd3dImmediateContext->RSSetState(RenderStates::RSNoCull.Get());
	m_pd3dImmediateContext->OMSetDepthStencilState(RenderStates::DSSDrawWithStencil.Get(), 1);
	m_pd3dImmediateContext->OMSetBlendState(RenderStates::BSTransparent.Get(), nullptr, 0xFFFFFFFF);

	DrawBoard();
	// 正面看的镜子
	DrawMirror();

	// 反射绘制完毕关掉反射参数
	m_VSConstantBuffer.isReflection = false;
	D3D11_MAPPED_SUBRESOURCE mappedData1;
	HR(m_pd3dImmediateContext->Map(m_pConstantBuffers[0].Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedData1));
	memcpy_s(mappedData1.pData, sizeof(VSConstantBuffer), &m_VSConstantBuffer, sizeof(VSConstantBuffer));
	m_pd3dImmediateContext->Unmap(m_pConstantBuffers[0].Get(), 0);

	// ******************
	// 4. 绘制不透明的正常物体
	//
	m_pd3dImmediateContext->RSSetState(nullptr);
	m_pd3dImmediateContext->OMSetDepthStencilState(nullptr, 0);
	m_pd3dImmediateContext->OMSetBlendState(nullptr, nullptr, 0xFFFFFFFF);

	DrawName();
	// ******************
	// 5. 绘制透明的正常物体
	//
	// 关闭顺逆时针裁剪
	// 透明混合
	m_pd3dImmediateContext->RSSetState(RenderStates::RSNoCull.Get());
	m_pd3dImmediateContext->OMSetDepthStencilState(nullptr, 0);
	m_pd3dImmediateContext->OMSetDepthStencilState(RenderStates::DSSWriteStencil.Get(), 1); 
	m_pd3dImmediateContext->OMSetBlendState(RenderStates::BSTransparent.Get(), nullptr, 0xFFFFFFFF);

	DrawBoard();

	HR(m_pSwapChain->Present(0, 0));
}


bool GameApp::InitEffect()
{
	ComPtr<ID3DBlob> blob;

	// 创建顶点着色器
	HR(CreateShaderFromFile(L"HLSL\\Light_VS.cso", L"HLSL\\Light_VS.hlsl", "VS", "vs_5_0", blob.ReleaseAndGetAddressOf()));
	HR(m_pd3dDevice->CreateVertexShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, m_pVertexShader.GetAddressOf()));
	// 创建并绑定顶点布局
	// 错误示范（，以作警示
	//HR(m_pd3dDevice->CreateInputLayout(VertexPosNormalTex::inputLayout, ARRAYSIZE(VertexPosNormalTex::inputLayout),
	//	blob->GetBufferPointer(), blob->GetBufferSize(), m_pVertexLayout.GetAddressOf()));
	HR(m_pd3dDevice->CreateInputLayout(VertexPosNormalColor::inputLayout, ARRAYSIZE(VertexPosNormalColor::inputLayout),
		blob->GetBufferPointer(), blob->GetBufferSize(), m_pVertexLayout.GetAddressOf()));


	HR(CreateShaderFromFile(L"HLSL\\GS.cso", L"HLSL\\GS.hlsl", "GS", "gs_5_0", blob.ReleaseAndGetAddressOf()));
	HR(m_pd3dDevice->CreateGeometryShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, m_pGeometryShader.GetAddressOf()));

	// 创建像素着色器
	HR(CreateShaderFromFile(L"HLSL\\Light_PS.cso", L"HLSL\\Light_PS.hlsl", "PS", "ps_5_0", blob.ReleaseAndGetAddressOf()));
	HR(m_pd3dDevice->CreatePixelShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, m_pPixelShader.GetAddressOf()));

	// 新增部分，用于在墙上绘制纹理另写的shader
	HR(CreateShaderFromFile(L"HLSL\\Basic_VS_3D.cso", L"HLSL\\Basic_VS_3D.hlsl", "VS_3D", "vs_5_0", blob.ReleaseAndGetAddressOf()));
	HR(m_pd3dDevice->CreateVertexShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, m_pVertexShaderTex.GetAddressOf()));
	// 创建顶点布局(TeX)
	HR(m_pd3dDevice->CreateInputLayout(VertexPosNormalTex::inputLayout, ARRAYSIZE(VertexPosNormalTex::inputLayout),
		blob->GetBufferPointer(), blob->GetBufferSize(), m_pVertexLayoutTex.GetAddressOf()));
	// 创建像素着色器(3D)
	HR(CreateShaderFromFile(L"HLSL\\Basic_PS_3D.cso", L"HLSL\\Basic_PS_3D.hlsl", "PS_3D", "ps_5_0", blob.ReleaseAndGetAddressOf()));
	HR(m_pd3dDevice->CreatePixelShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, m_pPixelShaderTex.GetAddressOf()));

	return true;
}

bool GameApp::InitResource()
{
	// 新增部分
	// 借用Geometry里的模板创建了一个新的CreateName方法，名字的所有数据存放在了Geometry.h里
	auto meshData = Geometry::CreateName<VertexPosNormalTex>();
	ResetMesh(meshData);

	// ******************
	// 设置常量缓冲区描述
	//
	D3D11_BUFFER_DESC cbd;
	ZeroMemory(&cbd, sizeof(cbd));
	cbd.Usage = D3D11_USAGE_DYNAMIC;
	cbd.ByteWidth = sizeof(VSConstantBuffer);
	cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	// 新建用于VS和PS的常量缓冲区
	HR(m_pd3dDevice->CreateBuffer(&cbd, nullptr, m_pConstantBuffers[0].GetAddressOf()));
	cbd.ByteWidth = sizeof(PSConstantBuffer);
	HR(m_pd3dDevice->CreateBuffer(&cbd, nullptr, m_pConstantBuffers[1].GetAddressOf()));

	// ******************
	// 初始化纹理和采样器状态
	//
	HR(CreateDDSTextureFromFile(m_pd3dDevice.Get(), L"Texture\\Smile.dds", nullptr, m_pWoodCrate.GetAddressOf()));
	HR(CreateDDSTextureFromFile(m_pd3dDevice.Get(), L"Texture\\ice.dds", nullptr, m_pIceCrate.GetAddressOf()));


	// 初始化采样器状态
	D3D11_SAMPLER_DESC sampDesc;
	ZeroMemory(&sampDesc, sizeof(sampDesc));
	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sampDesc.MinLOD = 0;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
	HR(m_pd3dDevice->CreateSamplerState(&sampDesc, m_pSamplerState.GetAddressOf()));


	// ******************
	// 新增部分
	// 初始化默认光照
	// 方向光
	m_DirLight.ambient = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	m_DirLight.diffuse = XMFLOAT4(0.8f, 0.8f, 0.8f, 1.0f);
	m_DirLight.specular = XMFLOAT4(1.5f, 1.0f, 1.5f, 1.0f);
	m_DirLight.direction = XMFLOAT3(0.577f, -1.677f, 0.577f);// 这里的y方向比较大，在矩阵上方和下放看的时候反光和不反光效果更明显

	// 萤火虫
	// 大圆灯照亮全局
	m_PointLight[0].position = XMFLOAT3(50.0f, 50.0f, 0.0f);
	m_PointLight[0].ambient = XMFLOAT4(0.4f, 0.4f, 0.4f, 1.0f);
	m_PointLight[0].diffuse = XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);// 衰减因子置为1，使点光源退化为纯粹的环境光
	m_PointLight[0].specular = XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);
	m_PointLight[0].att = XMFLOAT3(1.0f, 1.0f, 1.0f);
	m_PointLight[0].range = 5000.0f;// 注意这里的范围，一定要和高度配合好，不然就会被切割了，没有办法看到效果了，range就是点光源形成的圆的范围

	// 新增部分
	// 下面两个萤火虫的初始位置不重要，因为后续会更新，直接设为0
	m_PointLight[1].position = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_PointLight[1].ambient = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	m_PointLight[1].diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	m_PointLight[1].specular = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	m_PointLight[1].att = XMFLOAT3(0.5f, 0.5f, 0.5f);
	m_PointLight[1].range = 50.0f;

	m_PointLight[2].position = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_PointLight[2].ambient = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	m_PointLight[2].diffuse = XMFLOAT4(0.5f, 0.9f, 0.5f, 1.0f);
	m_PointLight[2].specular = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	m_PointLight[2].att = XMFLOAT3(0.5f, 0.5f, 0.5f);
	m_PointLight[2].range = 50.0f;

	// 新增部分
	// 探照灯
	m_SpotLight.position = XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_SpotLight.direction = XMFLOAT3(0.0f, 0.0f, 1.0f);
	m_SpotLight.ambient = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	m_SpotLight.diffuse = XMFLOAT4(0.9f, 0.3f, 0.9f, 1.0f);
	m_SpotLight.specular = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	m_SpotLight.att = XMFLOAT3(1.0f, 0.0f, 0.0f);// z方向，也就是朝前的方向不衰减效果比较明显
	m_SpotLight.spot = 15.0f;// spot越大越汇集,可见视野越小
	m_SpotLight.range = 5000.0f;

	// 初始化用于VS的常量缓冲区的值
	m_VSConstantBuffer.world = XMMatrixIdentity();			
	m_VSConstantBuffer.worldInvTranspose = XMMatrixInverse(nullptr, XMMatrixTranspose(m_VSConstantBuffer.world));	// 两次转置可以抵消
	m_VSConstantBuffer.view = XMMatrixTranspose(XMMatrixLookAtLH(
		XMVectorSet(0.0f, 0.0f, -5.0f, 0.0f),
		XMVectorSet(10.0f, 0.0f, 0.0f, 0.0f),
		XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f)
	));
	m_VSConstantBuffer.proj = XMMatrixTranspose(XMMatrixPerspectiveFovLH(XM_PIDIV2, AspectRatio(), 1.0f, 1000.0f));
	m_VSConstantBuffer.worldInvTranspose = XMMatrixIdentity();
	m_VSConstantBuffer.reflection = XMMatrixTranspose(XMMatrixReflect(XMVectorSet(0.0f, 0.0f, -1.0f, realMirrorPos)));
	m_VSConstantBuffer.isReflection = false;
	
	// 初始化用于PS的常量缓冲区的值
	m_PSConstantBuffer.material.ambient = XMFLOAT4(0.5f, 0.5f, 0.5f, 3.0f);
	m_PSConstantBuffer.material.diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 3.0f);
	m_PSConstantBuffer.material.specular = XMFLOAT4(0.5f, 0.5f, 0.5f, 5.0f);// 5.0决定了光泽度。最后一项决定的
	// 使用默认平行光
	m_PSConstantBuffer.dirLight = m_DirLight;
	// 注意不要忘记设置此处的观察位置，否则高亮部分会有问题
	m_PSConstantBuffer.eyePos = XMFLOAT4(10.0f, 0.0f, -5.0f, 0.0f);

	// 更新PS常量缓冲区资源
	D3D11_MAPPED_SUBRESOURCE mappedData;
	HR(m_pd3dImmediateContext->Map(m_pConstantBuffers[0].Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedData));
	memcpy_s(mappedData.pData, sizeof(VSConstantBuffer), &m_VSConstantBuffer, sizeof(VSConstantBuffer));
	m_pd3dImmediateContext->Unmap(m_pConstantBuffers[0].Get(), 0);

	HR(m_pd3dImmediateContext->Map(m_pConstantBuffers[1].Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedData));
	memcpy_s(mappedData.pData, sizeof(PSConstantBuffer), &m_VSConstantBuffer, sizeof(PSConstantBuffer));
	m_pd3dImmediateContext->Unmap(m_pConstantBuffers[1].Get(), 0);

	// ******************
	// 初始化光栅化状态
	//
	D3D11_RASTERIZER_DESC rasterizerDesc;
	ZeroMemory(&rasterizerDesc, sizeof(rasterizerDesc));
	rasterizerDesc.FillMode = D3D11_FILL_WIREFRAME;
	rasterizerDesc.CullMode = D3D11_CULL_NONE;
	rasterizerDesc.FrontCounterClockwise = false;
	rasterizerDesc.DepthClipEnable = true;
	HR(m_pd3dDevice->CreateRasterizerState(&rasterizerDesc, m_pRSWireframe.GetAddressOf()));

	// 初始化所有渲染状态
	RenderStates::InitAll(m_pd3dDevice.Get());

	// ******************
	// 给渲染管线各个阶段绑定好所需资源
	//

	// 设置图元类型，设定输入布局
	m_pd3dImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	m_pd3dImmediateContext->IASetInputLayout(m_pVertexLayout.Get());
	// 将着色器绑定到渲染管线
	m_pd3dImmediateContext->VSSetShader(m_pVertexShader.Get(), nullptr, 0);
	// 将集合着色器绑定到渲染管线
	// VS常量缓冲区对应HLSL寄存于b0的常量缓冲区
	m_pd3dImmediateContext->VSSetConstantBuffers(0, 1, m_pConstantBuffers[0].GetAddressOf());

	m_pd3dImmediateContext->GSSetShader(m_pGeometryShader.Get(), nullptr, 0);
	m_pd3dImmediateContext->GSSetConstantBuffers(0, 1, m_pConstantBuffers[0].GetAddressOf());
	// PS常量缓冲区对应HLSL寄存于b1的常量缓冲区

	m_pd3dImmediateContext->PSSetConstantBuffers(1, 1, m_pConstantBuffers[1].GetAddressOf());
	m_pd3dImmediateContext->PSSetSamplers(0, 1, m_pSamplerState.GetAddressOf());
	m_pd3dImmediateContext->PSSetShaderResources(0, 1, m_pWoodCrate.GetAddressOf());
	m_pd3dImmediateContext->PSSetShader(m_pPixelShader.Get(), nullptr, 0);


	// ******************
	// 设置调试对象名
	//
	D3D11SetDebugObjectName(m_pVertexLayout.Get(), "VertexPosNormalTexLayout");
	D3D11SetDebugObjectName(m_pConstantBuffers[0].Get(), "VSConstantBuffer");
	D3D11SetDebugObjectName(m_pConstantBuffers[1].Get(), "PSConstantBuffer");
	D3D11SetDebugObjectName(m_pVertexShader.Get(), "Light_VS");
	D3D11SetDebugObjectName(m_pPixelShader.Get(), "Light_PS");
	D3D11SetDebugObjectName(m_pGeometryShader.Get(), "GS");

	return true;
}


bool GameApp::ResetMesh(const Geometry::MeshData<VertexPosNormalTex>& meshData)
{
	// 释放旧资源
	m_pVertexBuffer.Reset();
	m_pIndexBuffer.Reset();

	// 设置顶点缓冲区描述
	D3D11_BUFFER_DESC vbd;
	ZeroMemory(&vbd, sizeof(vbd));
	vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = (UINT)meshData.vertexVec.size() * sizeof(VertexPosNormalTex);
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	// 新建顶点缓冲区
	D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory(&InitData, sizeof(InitData));
	InitData.pSysMem = meshData.vertexVec.data();
	HR(m_pd3dDevice->CreateBuffer(&vbd, &InitData, m_pVertexBuffer.GetAddressOf()));

	// 输入装配阶段的顶点缓冲区设置
	UINT stride = sizeof(VertexPosNormalTex);	// 跨越字节数
	UINT offset = 0;							// 起始偏移量

	m_pd3dImmediateContext->IASetVertexBuffers(0, 1, m_pVertexBuffer.GetAddressOf(), &stride, &offset);



	// 设置索引缓冲区描述
	m_IndexCount = (UINT)meshData.indexVec.size();
	D3D11_BUFFER_DESC ibd;
	ZeroMemory(&ibd, sizeof(ibd));
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = m_IndexCount * sizeof(WORD);
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	// 新建索引缓冲区
	InitData.pSysMem = meshData.indexVec.data();
	HR(m_pd3dDevice->CreateBuffer(&ibd, &InitData, m_pIndexBuffer.GetAddressOf()));
	// 输入装配阶段的索引缓冲区设置
	m_pd3dImmediateContext->IASetIndexBuffer(m_pIndexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0);



	// 设置调试对象名
	D3D11SetDebugObjectName(m_pVertexBuffer.Get(), "VertexBuffer");
	D3D11SetDebugObjectName(m_pIndexBuffer.Get(), "IndexBuffer");


	return true;
}
