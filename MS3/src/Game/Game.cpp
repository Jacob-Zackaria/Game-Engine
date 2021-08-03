//----------------------------------------------------------------------------
// Copyright 2020, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "ShaderObject.h"
#include "MathEngine.h"
#include "GameManager.h"
#include "GameObject.h"
#include "CubeModel.h"
#include "PyramidModel.h"
#include "EdPyramidModel.h"
#include "DiamondModel.h"
#include "SphereModel.h"
#include "CrossModel.h"
#include "SpaceShipModel.h"
#include "AstroBoy.h"
#include "MilitaryModel.h"
#include "WarBearModel.h"
#include "MissileModel.h"
#include "GraphicsObject.h"
#include "GraphicsObject_ColorByPosition.h"
#include "GraphicsObject_Wireframe.h"
#include "GraphicsObject_WireframeConstantColor.h"
#include "GraphicsObject_FlatTexture.h"
#include "GraphicsObject_TextureLight.h"
#include "GraphicsObject_ConstColor.h"
#include "GraphicsObject_FontSprite.h"
#include "TextureManager.h"
#include "InputManager.h"
#include "CamerasManager.h"
#include "Camera3D.h"
#include "Camera2D.h"
#include "GameObjectManager.h"
#include "ModelManager.h"
#include "ShaderManager.h"
#include "BoundingSphereGameObject.h"
#include "CameraModel.h"
#include "CameraDemoInput.h"
#include "GameObjectAnimation.h"
#include "AnimTime.h"
#include "AnimTimer.h"
#include "VectApp.h"
#include "QuatApp.h"
#include "Animation.h"
#include "GraphicsDebugMessage.h"
#include "TimerController.h"
#include "AnimationController.h"
#include "Clip.h"
#include "Skeleton.h"
#include "AnimationManager.h"
#include "ImageManager.h"
#include "GraphicsObject_Sprite.h"
#include "SpriteModel.h"
#include "GameObject2D.h"
#include "GameObject3D.h"
#include "GameObjectFont.h"
#include "GlyphManager.h"
#include "FontManager.h"
#include "TeddyModel2.h"
#include "TeddyModel.h"
#include "HumanModel.h"
#include "GraphicsObject_SkinTexture.h"
#include "GraphicsObject_SkinSimpleColor.h"
#include "GraphicsObject_SkinConstantColor.h"
#include "GameObjectRigid.h"
#include "SSBO.h"
#include "AnimationTransition.h"

namespace Azul
{
	// TO DO: ----------------------------------

	// Demo
	Vect Orig_up;
	Vect Orig_tar;
	Vect Orig_pos;
	Vect Orig_upNorm;
	Vect Orig_forwardNorm;
	Vect Orig_RightNorm;
	CameraModel* pCameraModel;
	
	//-----------------------------------------------------------------------------
	//  Game::Game()
	//		Game Engine Constructor
	//-----------------------------------------------------------------------------
	Game::Game(const char* pWindowName, const int Width, const int Height)
		:
		Engine(pWindowName, Width, Height)
	{
		assert(pWindowName);
		this->globalTimer.Tic();
		this->intervalTimer.Tic();
	}

	//-----------------------------------------------------------------------------
	// Game::Initialize()
	//		Allows the engine to perform any initialization it needs to before 
	//      starting to run.  This is where it can query for any required services 
	//      and load any non-graphic related content. 
	//-----------------------------------------------------------------------------
	void Game::Initialize()
	{
	}
	GameObject3D* pGameObjCube;
	BoundingSphereGameObject* pGameObjSphere;
	GameObject2D* pA1;
	GameObject2D* pA2;
	//-----------------------------------------------------------------------------
	// Game::LoadContent()
	//		Allows you to load all content needed for your engine,
	//	    such as objects, graphics, etc.
	//-----------------------------------------------------------------------------
	void Game::LoadContent()
	{
		//---------------------- Start the managers -----------------------//
		AnimationManager::Create();
		InputManager::Create();
		ModelManager::Create();
		ShaderManager::Create();
		CamerasManager::Create();
		TextureManager::Create();
		ImageManager::Create();
		GameObjectManager::Create();
		GlyphManager::Create();
		FontManager::Create();
		//----------------------------------------------------------------//

		//---------------------- Initialize Camera -----------------------//
		// Camera setup

		Camera3D* pCam0 = new Camera3D();
		pCam0->setViewport(0, 0, this->info.windowWidth, this->info.windowHeight);

		// For Human
		pCam0->setPerspective(35.0f, float(pCam0->getScreenWidth()) / float(pCam0->getScreenHeight()), 1.0f, 20000.0f);
		pCam0->setOrientAndPosition(Vect(0.0f, 0.0f, 1.0f), Vect(0.0f, 0.0f, 0.0f), Vect(2000.0f, 0.0f, 0.0f));

		// For Teddy
	/*	pCam0->setPerspective(35.0f, float(pCam0->getScreenWidth()) / float(pCam0->getScreenHeight()), 1.0f, 20000.0f);
		pCam0->setOrientAndPosition(Vect(0.0f, 0.0f, 1.0f), Vect(0.0f, 0.0f, 100.0f), Vect(450.0f, 0.0f, 0.0f));*/

		pCam0->updateCamera();
		CamerasManager::Add(pCam0, Camera::Name::CAMERA_0);
		CamerasManager::SetCurrent3D(Camera::Name::CAMERA_0);

		//---------------------------------------------------------------------------------------------------------
		// Setup the current 2D orthographic Camera
		//---------------------------------------------------------------------------------------------------------
		Camera2D* pCam2D = new Camera2D();
		pCam2D->setViewport(0, 0, this->info.windowWidth, this->info.windowHeight);
		pCam2D->setOrthographic((float)-pCam2D->getScreenWidth() / 2.0f, (float)pCam2D->getScreenWidth() / 2.0f,
			(float)-pCam2D->getScreenHeight() / 2.0f, (float)pCam2D->getScreenHeight() / 2.0f,
			1.0f, 1000.0f);
		pCam2D->setOrientAndPosition(Vect(0.0f, 1.0f, 0.0f), Vect(0.0f, 0.0f, -1.0f), Vect(0.0f, 0.0f, 2.0f));

		// Holder for the current 2D  cameras
		CamerasManager::Add(pCam2D, Camera::Name::CAMERA_1);
		CamerasManager::SetCurrent2D(Camera::Name::CAMERA_1);

		//--------------------------- Load the model -----------------------//

		SpriteModel* pSpriteModel = new SpriteModel("./Models/SpriteModel.azul");
		assert(pSpriteModel);
		ModelManager::Add(pSpriteModel);

		TeddyModel* pTeddyModel = new TeddyModel("../../Models/teddyModel.skin");
		assert(pTeddyModel);
		ModelManager::Add(pTeddyModel);

		// Human
		HumanModel* pHumanModel = new HumanModel("../../Models/humanoidModel.skin");
		assert(pHumanModel);
		ModelManager::Add(pHumanModel);

		// Space ship model
		SpaceShipModel* pSpaceModel = new SpaceShipModel("../../Models/spaceModel2.azul");
		assert(pSpaceModel);
		ModelManager::Add(pSpaceModel);

		// Astro boy model
		AstroBoy* pAstroModel = new AstroBoy("../../Models/astroModel2.azul");
		assert(pAstroModel);
		ModelManager::Add(pAstroModel);

		// Sphere model
		SphereModel* pSphereModel = new SphereModel("./Models/sphereModel.azul");
		assert(pSphereModel);
		ModelManager::Add(pSphereModel);


		//------------------------- Create/Load Shader ---------------------//

		ShaderObject* pShaderObject_skinTexture = new ShaderObject(ShaderObject::Name::SKIN_TEXTURE, "./Shaders/skinTextureRender", ShaderObject::Type::GRAPHICS);
		assert(pShaderObject_skinTexture);
		ShaderManager::Add(pShaderObject_skinTexture);

		ShaderObject* pShaderObject_skinSimpleColor = new ShaderObject(ShaderObject::Name::SKIN_SIMPLE_COLOR, "./Shaders/skinSimpleColorRenderer", ShaderObject::Type::GRAPHICS);
		assert(pShaderObject_skinSimpleColor);
		ShaderManager::Add(pShaderObject_skinSimpleColor);

		ShaderObject* pShaderObject_skinConstantColor = new ShaderObject(ShaderObject::Name::SKIN_CONSTANT_COLOR, "./Shaders/skinColorRender", ShaderObject::Type::GRAPHICS);
		assert(pShaderObject_skinConstantColor);
		ShaderManager::Add(pShaderObject_skinConstantColor);

		ShaderObject* pShaderObject_sprite = new ShaderObject(ShaderObject::Name::SPRITE, "./Shaders/spriteRender", ShaderObject::Type::GRAPHICS);
		assert(pShaderObject_sprite);
		ShaderManager::Add(pShaderObject_sprite);

		// Simple texture shader.
		ShaderObject* pShaderObject_texture = new ShaderObject(ShaderObject::Name::TEXTURE_SIMPLE, "./Shaders/textureRender", ShaderObject::Type::GRAPHICS);
		assert(pShaderObject_texture);
		ShaderManager::Add(pShaderObject_texture);

		// Single color
		ShaderObject* pShaderObject_colorSingle = new ShaderObject(ShaderObject::Name::COLOR_SINGLE, "./Shaders/colorSingleRender", ShaderObject::Type::GRAPHICS);
		assert(pShaderObject_colorSingle);
		ShaderManager::Add(pShaderObject_colorSingle);

		// Compute Shader
		ShaderObject* pShaderObject_mixer = new ShaderObject(ShaderObject::Name::COMPUTE_MIXER, "./Shaders/computeMixer", ShaderObject::Type::COMPUTE);
		assert(pShaderObject_mixer);
		ShaderManager::Add(pShaderObject_mixer);

		ShaderObject* pShaderObject_world = new ShaderObject(ShaderObject::Name::COMPUTE_WORLD, "./Shaders/computeWorld", ShaderObject::Type::COMPUTE);
		assert(pShaderObject_world);
		ShaderManager::Add(pShaderObject_world);

		//------------- TEXTURE ----------------//

		TextureManager::Add("./Assets/Rocks.tga", Texture::Name::ROCKS);
		TextureManager::Add("./Assets/Stone.tga", Texture::Name::STONES);
		TextureManager::Add("./Assets/RedBrick.tga", Texture::Name::RED_BRICK);
		TextureManager::Add("./Assets/Duckweed.tga", Texture::Name::DUCKWEED);
		TextureManager::Add("./Assets/Aliens.tga", Texture::Name::INVADERS);
		TextureManager::Add("./Assets/TeddyTexNORLE.tga", Texture::Name::TEDDY);

		//------FONT---------------///
		TextureManager::Add("../../Fonts/Consolas36pt.tga", Texture::Name::CONSOLAS_FONT);
		GlyphManager::AddXml(Glyph::Name::CONSOLAS_FONT, "../../Fonts/Consolas36pt.act", Texture::Name::CONSOLAS_FONT);

		
		Font* newFont = new Font("TEST 123", Glyph::Name::CONSOLAS_FONT);
		FontManager::Add(newFont, Font::Name::CONSOLAS_FONT);
		GraphicsObject_FontSprite* pFontSprite = new GraphicsObject_FontSprite(pSpriteModel, pShaderObject_sprite, newFont, Rectangle(150, 300, 60, 60));
		GameObjectFont* pAf = new GameObjectFont(pFontSprite);
		GameObjectManager::Add(pAf, GameObjectManager::GetRoot());
		pAf->SetName("CONSOLA_FONTTT");
		pAf->posX = 20.0f;
		pAf->posY = 20.0f;
		pAf->scaleX = 0.5f;
		pAf->scaleY = 0.5f;
		//--------------------//

		//------FONT---------------///
		TextureManager::Add("../../Fonts/Bradley_30pt.tga", Texture::Name::BRADLEY_FONT);
		GlyphManager::AddXml(Glyph::Name::BRADLEY_FONT, "../../Fonts/Bradley_30pt.act", Texture::Name::BRADLEY_FONT);


		newFont = new Font("SKINNING : HUMANOID, TEDDY", Glyph::Name::BRADLEY_FONT);
		FontManager::Add(newFont, Font::Name::BRADLEY_FONT);
		pFontSprite = new GraphicsObject_FontSprite(pSpriteModel, pShaderObject_sprite, newFont, Rectangle(100, 200, 60, 60));
		pAf = new GameObjectFont(pFontSprite);
		GameObjectManager::Add(pAf, GameObjectManager::GetRoot());
		pAf->SetName("BR_FONTTT");
		pAf->posX = 0.0f;
		pAf->posY = 0.0f;
		//--------------------//


		//------IMAGE---------------///
		Image* pImageGreen = new Image(Image::Name::Alien_Green, Texture::Name::INVADERS, Rectangle(136.0f, 64.0f, 85.0f, 63.0f));
		Image* pImageRed = new Image(Image::Name::Alien_Red, Texture::Name::INVADERS, Rectangle(554.0f, 63.0f, 98.0f, 64.0f));

		assert(pImageGreen);
		ImageManager::Add(pImageGreen);
		ImageManager::Add(pImageRed);

		//------SPRITE---------------///
		GraphicsObject_Sprite* pGraphics_Sprite = new GraphicsObject_Sprite(pSpriteModel, pShaderObject_sprite, pImageGreen, Rectangle(455, 155, 60, 60));
		pA1 = new GameObject2D(pGraphics_Sprite);
		GameObjectManager::Add(pA1, GameObjectManager::GetRoot());
		pA1->posX = 1070.0f;
		pA1->posY = 860.0f;
		pA1->SetName("ALIEN");

		//------SPRITE---------------///
		pGraphics_Sprite = new GraphicsObject_Sprite(pSpriteModel, pShaderObject_sprite, pImageRed, Rectangle(600, 155, 60, 60));
		pA2 = new GameObject2D(pGraphics_Sprite);
		GameObjectManager::Add(pA2, GameObjectManager::GetRoot());
		pA2->posX = 1070.0f;
		pA2->posY = 40.0f;
		pA2->SetName("ALIEN_RED");

		//------GAMEOBJECT 3D SPACESHIP---------------///
		GraphicsObject* pGraphics_e = new GraphicsObject_FlatTexture(pSpaceModel, pShaderObject_texture, Texture::Name::SPACE_SHIP, GL_CCW);
		pGameObjCube = new GameObject3D(pGraphics_e);
		pGameObjCube->SetName("GreenCube");
		pGameObjCube->SetScale(4.5f, 4.5f, 4.5f);
		pGameObjCube->SetPos(-1800.0f, 0.0f, 500.0f);
		pGameObjCube->SetRot(0.001f, 0.0f, 0.001f);
		//pGameObjCube->SetOrient(0.0f, 0.0f, 0.0f);
		//pGameObjCube->setMove(SimpleGameObject::Move_Y);
		GameObjectManager::Add(pGameObjCube, GameObjectManager::GetRoot());

		// ----- BOUNDING SPHERE ------------//

		Vect Yellow(1.0f, 1.0f, 0.0f, 1.0f);
		pGraphics_e = new GraphicsObject_WireframeConstantColor(pSphereModel, pShaderObject_colorSingle, Yellow);

		pGameObjSphere = new BoundingSphereGameObject(pGraphics_e);
		pGameObjSphere->SetName("BoundingSphere_GREEN");
		pGameObjSphere->SetBoundingSphereReference(pGameObjCube);
		GameObjectManager::Add(pGameObjSphere, GameObjectManager::GetRoot());

		//------GAME OBJECT 3D ASTRO---------------///
		pGraphics_e = new GraphicsObject_FlatTexture(pAstroModel, pShaderObject_texture, Texture::Name::ASTRO_BOY, GL_CCW);
		pGameObjCube = new GameObject3D(pGraphics_e);
		pGameObjCube->SetName("GreenCube");
		pGameObjCube->SetScale(5.5f, 5.5f, 5.5f);
		//pGameObjCube->SetOrient(0.0f, 0.0f, 0.0f);
		//pGameObjCube->setMove(SimpleGameObject::Move_Y);
		GameObjectManager::Add(pGameObjCube, GameObjectManager::GetRoot());

		// ----- BOUNDING SPHERE ------------//

		pGraphics_e = new GraphicsObject_WireframeConstantColor(pSphereModel, pShaderObject_colorSingle, Yellow);

		pGameObjSphere = new BoundingSphereGameObject(pGraphics_e);
		pGameObjSphere->SetName("BoundingSphere_GREEN");
		pGameObjSphere->SetBoundingSphereReference(pGameObjCube);
		GameObjectManager::Add(pGameObjSphere, GameObjectManager::GetRoot());
		//-------------------------------------------------------------------//

		//Create Skeleton.
		// Start location
		Vect startLocTeddy = Vect(0.0f, 70.0f, 0.0f);
		Vect angleTeddy = Vect(MATH_PI2, 0.0f, 0.0f);
		Skeleton* skeletonTeddy = new Skeleton("../../Models/Hierarchy4.skel", angleTeddy, startLocTeddy);
		AnimationManager::AddSkeleton(skeletonTeddy);

		//------GAME OBJECT 3D TEDDY---------------///
		GraphicsObject_SkinTexture* pSkinGraphicsObject = new GraphicsObject_SkinTexture(pTeddyModel, pShaderObject_skinTexture, Texture::Name::TEDDY, skeletonTeddy->GetGameRigid());
		GameObject* pSkinObject = new GameObjectRigid(pSkinGraphicsObject);
		GameObjectManager::Add(pSkinObject, GameObjectManager::GetRoot());
		pSkinObject->SetName("Skin Object");

		//Create Skeleton.
		// Start location
		Vect startLocHuman = Vect(0.0f, 20.0f, 0.0f);
		Vect angleHuman = Vect(0.0f, 0.0f, 0.0f);
		Skeleton* newHSkeleton = new Skeleton("../../Models/Hierarchy2.skel", angleHuman, startLocHuman);
		AnimationManager::AddSkeleton(newHSkeleton);

		//------GAME OBJECT 3D HUMAN---------------///
		Vect redcolor = Vect(1.0f, 0.0f, 0.0f);
		Vect lightPos = Vect(0.0f, 1.0f, 0.0f);
		GraphicsObject_SkinConstantColor* pSkinSimpleColor = new GraphicsObject_SkinConstantColor(pHumanModel, pShaderObject_skinConstantColor, redcolor, lightPos, newHSkeleton->GetGameRigid());
		GameObjectRigid* pHumanSkinObject = new GameObjectRigid(pSkinSimpleColor);
		GameObjectManager::Add(pHumanSkinObject, GameObjectManager::GetRoot());
		pHumanSkinObject->SetName("Skin Object Human");




		//----------------------- ANIMATION TEDDY ----------------------//

		
		int numOfBones = skeletonTeddy->GetNumBones();
		Clip* pWalkClip = new Clip(numOfBones, 10.0f, "../../Models/Frames4.clip", Clip::Name::TEDDY_WALK, 0);
		assert(pWalkClip);
		AnimationManager::AddClip(pWalkClip);

		Clip* pIdleClip = new Clip(numOfBones, 10.0f, "../../Models/Frames4.clip", Clip::Name::TEDDY_IDLE, 1);
		assert(pIdleClip);
		AnimationManager::AddClip(pIdleClip);


		// Anim Controller A
		AnimTime delta = 0.1f * AnimTime(AnimTime::Duration::NTSC_30_FRAME);
		AnimationController* pAnimControllerA = new AnimationController(delta, Clip::Name::TEDDY_WALK);
		AnimationManager::Add(pAnimControllerA, AnimationController::Name::TEDDY);

		// Anim Controller B
		delta = 0.1f * AnimTime(AnimTime::Duration::NTSC_30_FRAME);
		AnimationController* pAnimControllerB = new AnimationController(delta, Clip::Name::TEDDY_IDLE);
		AnimationManager::Add(pAnimControllerB, AnimationController::Name::TEDDY);

		// Transition controller
		AnimationTransition* pAnimTransition = new AnimationTransition(skeletonTeddy->GetTable(), pWalkClip, pIdleClip, 0.0f);
		pSkinGraphicsObject->pBoneWord_SBO = pAnimTransition->poBoneWorld_SBO;
		AnimationManager::AddTransition(pAnimTransition, AnimationTransition::Name::TEDDY_WALK_IDLE);

		//----------------------- ANIMATION HUMAN ----------------------//
		
		
		int numOfHBones = newHSkeleton->GetNumBones();
		Clip* pHRunClip = new Clip(numOfHBones, 10.0f, "../../Models/Frames2.clip", Clip::Name::HUMANOID_RUN, 0);
		assert(pHRunClip);
		AnimationManager::AddClip(pHRunClip);

		Clip* pHPunchClip = new Clip(numOfHBones, 10.0f, "../../Models/Frames2.clip", Clip::Name::HUMANOID_PUNCH, 1);
		assert(pHPunchClip);
		AnimationManager::AddClip(pHPunchClip);

		Clip* pHShotClip = new Clip(numOfHBones, 10.0f, "../../Models/Frames2.clip", Clip::Name::HUMANOID_SHOT, 2);
		assert(pHShotClip);
		AnimationManager::AddClip(pHShotClip);

		// Anim Controller
		AnimTime Hdelta = 0.1f * AnimTime(AnimTime::Duration::NTSC_30_FRAME);
		AnimationController* pAnimController = new AnimationController(Hdelta, Clip::Name::HUMANOID_RUN);
		AnimationManager::Add(pAnimController, AnimationController::Name::HUMANOID_1);

		// Anim Controller
		Hdelta = 0.1f * AnimTime(AnimTime::Duration::NTSC_30_FRAME);
		AnimationController* pAnimControllerX = new AnimationController(Hdelta, Clip::Name::HUMANOID_PUNCH);
		AnimationManager::Add(pAnimControllerX, AnimationController::Name::HUMANOID_2);


		// Transition controller
		AnimationTransition* pAnimTransition2 = new AnimationTransition(newHSkeleton->GetTable(), pHRunClip, pHPunchClip, 0.0f);
		pSkinSimpleColor->pBoneWord_SBO = pAnimTransition2->poBoneWorld_SBO;
		AnimationManager::AddTransition(pAnimTransition2, AnimationTransition::Name::HUMAN_RUN_PUNCH);

	
		//--------------------------------------------------------//

		// -------------------------------------------------------
		// TOGGLE between cameras...
		//
		// Key 0 - camera0 under investigation
		// Key 1, 2, 3 different views of camera 0
		// -------------------------------------------------------
		Trace::out("\n\n Hot Keys:");
		Trace::out("\n Z: Reset Camera");
		Trace::out("\n W: Move Forward");
		Trace::out("\n S: Move Backward");
		Trace::out("\n Left Arrow: rotate left");
		Trace::out("\n Right Arrow: rotate right");
		Trace::out("\n K: Forward Blending");
		Trace::out("\n J: Reverse Blending");
		Trace::out("\n F: Play Forward");
		Trace::out("\n B: Play Reverse");
		Trace::out("\n G: Fast Forward");
		Trace::out("\n H: Play Slow");
		Trace::out("\n P: Pause Playback\n\n");

		CamerasManager::GetActiveHelper(Orig_up, Orig_tar, Orig_pos, Orig_upNorm, Orig_forwardNorm, Orig_RightNorm);

	}

	Vect pos;
	Vect tar;
	Vect up;
	Vect upNorm;
	Vect forwardNorm;
	Vect rightNorm;
	Vect obj(0.0f, 0.0f, 0.0f);
	//-----------------------------------------------------------------------------
	// Game::Update()
	//      Called once per frame, update data, tranformations, etc
	//      Use this function to control process order
	//      Input, AI, Physics, Animation, and Graphics
	//-----------------------------------------------------------------------------
	static float yax = 0.0f;
	static int count = 0;
	
	void Game::Update(float)
	{
		//---------------------------------------------------------------------------------------------------------
		// Font Experiment
		//---------------------------------------------------------------------------------------------------------
		{
			static std::string newS;
			
			count++;
			if (count >= 1200)
			{
				count = 0;
			}


			Font* pTestMessage = FontManager::Find(Font::Name::CONSOLAS_FONT);
			newS = "ANGLE ROT:" + std::to_string(count);
			pTestMessage->setString(newS.c_str());

		}

		//---------------------------------------------------------------------------------------------------------
		// Sprite Experiment
		//---------------------------------------------------------------------------------------------------------
		{
			pA2->posX -= 0.5f;
			if (pA2->posX <= 20.0f)
			{
				pA2->posX = 1070.0f;
			}
			pA1->posY -= 0.5f;
			if (pA1->posY <= 20.0f)
			{
				pA1->posY = 860.0f;
			}
		}

		Keyboard* pKey = InputManager::GetKeyboard();

		//---------------------------------------------------------------------------------------------------------
		// Camera Experiment (Move to Function)
		//---------------------------------------------------------------------------------------------------------
		{
			// Cam Control
			float camSpeed = 8.0f;
			float rotSpeed = 0.03f;

			// Reset Z
			if (pKey->GetKeyState(Keyboard::KEY_Z))
			{
				// Reset	
				obj.set(0, 0, 0);
				CamerasManager::SetActiveHelper(Orig_up, Orig_tar, Orig_pos);
			}

			// Move Forward W
			if (pKey->GetKeyState(Keyboard::KEY_W))
			{



				CamerasManager::GetActiveHelper(up, tar, pos, upNorm, forwardNorm, rightNorm);
				// OK, now 3 points... pos, tar, up
				//     now 3 normals... upNorm, forwardNorm, rightNorm
				//	tar = obj;

				pos += camSpeed * forwardNorm;
				tar += 0.03f * forwardNorm;


				CamerasManager::SetActiveHelper(up, tar, pos);
			}

			// Move Reverse S
			if (pKey->GetKeyState(Keyboard::KEY_S))
			{



				CamerasManager::GetActiveHelper(up, tar, pos, upNorm, forwardNorm, rightNorm);
				// OK, now 3 points... pos, tar, up
				//     now 3 normals... upNorm, forwardNorm, rightNorm
				tar = obj;

				pos -= camSpeed * forwardNorm;

				CamerasManager::SetActiveHelper(up, tar, pos);
			}

			// Rotate Right 
			if (pKey->GetKeyState(Keyboard::KEY_ARROW_RIGHT))
			{



				CamerasManager::GetActiveHelper(up, tar, pos, upNorm, forwardNorm, rightNorm);
				// OK, now 3 points... pos, tar, up
				//     now 3 normals... upNorm, forwardNorm, rightNorm
				tar = obj;

				Matrix Trans(Matrix::Trans::XYZ, tar);
				Matrix NegTrans(Matrix::Trans::XYZ, -tar);

				Matrix Rot;
				Rot.set(Matrix::Rot::AxisAngle, upNorm, rotSpeed);

				Matrix M = NegTrans * Rot * Trans;

				up = up * M;
				pos = pos * M;
				tar = tar * M;

				CamerasManager::SetActiveHelper(up, tar, pos);
			}

			// Rotate Left
			if (pKey->GetKeyState(Keyboard::KEY_ARROW_LEFT))
			{



				CamerasManager::GetActiveHelper(up, tar, pos, upNorm, forwardNorm, rightNorm);
				// OK, now 3 points... pos, tar, up
				//     now 3 normals... upNorm, forwardNorm, rightNorm

				tar = obj;

				Matrix Trans(Matrix::Trans::XYZ, tar);
				Matrix NegTrans(Matrix::Trans::XYZ, -tar);

				Matrix Rot;
				Rot.set(Matrix::Rot::AxisAngle, upNorm, -rotSpeed);

				Matrix M = NegTrans * Rot * Trans;

				up = up * M;
				pos = pos * M;
				tar = tar * M;

				CamerasManager::SetActiveHelper(up, tar, pos);
			}

			// Rotate Down
			if (pKey->GetKeyState(Keyboard::KEY_ARROW_DOWN))
			{


				CamerasManager::GetActiveHelper(up, tar, pos, upNorm, forwardNorm, rightNorm);
				tar = obj;

				// OK, now 3 points... pos, tar, up
				//     now 3 normals... upNorm, forwardNorm, rightNorm

				Matrix Trans(Matrix::Trans::XYZ, tar);
				Matrix NegTrans(Matrix::Trans::XYZ, -tar);
				Matrix Rot;
				Rot.set(Matrix::Rot::AxisAngle, rightNorm, rotSpeed);

				Matrix M = NegTrans * Rot * Trans;

				up = up * M;
				pos = pos * M;
				tar = tar * M;

				CamerasManager::SetActiveHelper(up, tar, pos);
			}

			// Rotate Up
			if (pKey->GetKeyState(Keyboard::KEY_ARROW_UP))
			{


				CamerasManager::GetActiveHelper(up, tar, pos, upNorm, forwardNorm, rightNorm);

				// OK, now 3 points... pos, tar, up
				//     now 3 normals... upNorm, forwardNorm, rightNorm
				tar = obj;

				Matrix Trans(Matrix::Trans::XYZ, tar);
				Matrix NegTrans(Matrix::Trans::XYZ, -tar);
				Matrix Rot;
				Rot.set(Matrix::Rot::AxisAngle, rightNorm, -rotSpeed);

				Matrix M = NegTrans * Rot * Trans;

				up = up * M;
				pos = pos * M;
				tar = tar * M;

				CamerasManager::SetActiveHelper(up, tar, pos);
			}
		} 

		// ------------------------------------------------
		// Update Camera
		// ------------------------------------------------
		CamerasManager::Update2D();
		CamerasManager::Update3D();
		
		
		// ------------------------------------------------
		// Animation
		// ------------------------------------------------

		AnimationManager::Update();
		AnimationTransition* pAnimTransitionTeddy = AnimationManager::Find(AnimationTransition::Name::TEDDY_WALK_IDLE);
		pAnimTransitionTeddy->Update();
		AnimationTransition* pAnimTransitionHuman = AnimationManager::Find(AnimationTransition::Name::HUMAN_RUN_PUNCH);
		pAnimTransitionHuman->Update();

		//---------------------------------------------------------------------------------------------------------
		// GameObject Experiment
		//---------------------------------------------------------------------------------------------------------
		{
			yax += 1.0f;

			if (yax >= 2000.0f)
				yax = 0.0f;

			pGameObjCube->SetPos(yax, 0.0f, 0.0f);
		}

		//---------------------------------------------------------------------------------------------------------
		// Animation Timer Experiment (Move to Function)
		//---------------------------------------------------------------------------------------------------------
		pAnimTransitionTeddy->StartTransition();
		pAnimTransitionHuman->StartTransition();
		if (pKey->GetKeyState(Keyboard::KEY_K))
		{
			pAnimTransitionTeddy->SetDelta(0.003f);
			pAnimTransitionHuman->SetDelta(0.003f);
		}

		if (pKey->GetKeyState(Keyboard::KEY_J))
		{
			pAnimTransitionTeddy->SetDelta(-0.003f);
			pAnimTransitionHuman->SetDelta(-0.003f);
		}
		// ------------------------------------------------
		// GameObjects update
		// ------------------------------------------------
		GameObjectManager::Update(this->globalTimer.Toc());

	}


	//-----------------------------------------------------------------------------
	// Game::Draw()
	//		This function is called once per frame
	//	    Use this for draw graphics to the screen.
	//      Only do rendering here
	//-----------------------------------------------------------------------------
	void Game::Draw()
	{
		GameObjectManager::Draw();
	}


	//-----------------------------------------------------------------------------
	// Game::UnLoadContent()
	//       unload content (resources loaded above)
	//       unload all content that was loaded before the Engine Loop started
	//-----------------------------------------------------------------------------
	void Game::UnLoadContent()
	{
		FontManager::Destroy();
		GlyphManager::Destroy();
		InputManager::Destroy();
		GameObjectManager::Destroy();
		ModelManager::Destroy();
		ShaderManager::Destroy();
		CamerasManager::Destroy();
		ImageManager::Destroy();
		TextureManager::Destroy();
		AnimationManager::Destroy();
	}


	//-----------------------------------------------------------------------------
	// Game::ScreenResize()
	//-----------------------------------------------------------------------------
	//void Game::onResize(int w, int h)
	//{
	//	Engine::onResize(w, h);
	//}


	//------------------------------------------------------------------
	// Game::ClearBufferFunc()
	// Allows user to change the way the clear buffer function works
	//------------------------------------------------------------------
	void Game::ClearBufferFunc()
	{
		const GLfloat grey[] = { 0.7f, 0.7f, 0.8f, 1.0f };
		const GLfloat one = 1.0f;

		glViewport(0, 0, info.windowWidth, info.windowHeight);
		glClearBufferfv(GL_COLOR, 0, grey);
		glClearBufferfv(GL_DEPTH, 0, &one);
	}

}
// --- End of File ---
