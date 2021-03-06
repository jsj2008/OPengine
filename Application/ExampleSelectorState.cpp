#include "./ExampleSelectorState.h"
#include "./Human/include/Input/OPinputRecord.h"


#ifdef ADDON_socketio
#include "OPsocketGamePadSystem.h"
#endif

#define TotalCategories 4
#define TotalEntries (ExampleCount + TotalCategories)

typedef struct {
	const OPchar* name;
	OPgameState* state;
	OPint available;
    OPint parent;
} Example;

class ExampleSelector : public OPgameState {
	OPint Selected;
    OPint Initialized;
	OPfontManager* FontManager;
	Example Examples[TotalEntries];
    OPint CurrentHierarchy;
    OPint HierarchyDepth[TotalEntries];
    OPint CurrentDepth;
	OPtexture2DOLD*Background;
	OPgamePad* Controller;
#ifdef ADDON_socketio
	OPsocketGamePad* SocketController;
#endif


	void Init(OPgameState* last) {

		//OPCMAN.LoadGet("cemetery.tga");

		OPCMAN.Load("Ubuntu.opf");

		OPfontSystemLoadEffects();


		// The background image to use
		const OPchar* bgTex = "test.bmp"; // "subtle-irongrip.png";
		OPCMAN.Load(bgTex);
		Background = OPtexture2DCreate((OPtexture*)OPCMAN.Get(bgTex), NULL, OPVEC2_ZERO, OPvec2(10.0f, 10.0f));
		Background->Scale = OPvec2(1.0f, 1.0f);

		if (!Initialized) {
			// This ensures that our menu selections stay in place
			// And we don't re-initialize the examples array for no reason
			Initialized = 1;
			Controller = OPGAMEPADS[0];
#ifdef ADDON_socketio
			SocketController = OPSOCKETGAMEPADS[0];
#endif

			Example examples[TotalEntries] = {
				// Categories
				{ "Basics", NULL, 1, -1 },
				{ "Intermediate", NULL, 1, -1 },
				{ "Advanced", NULL, 1, -1 },
				{ "Exit", NULL, 1, -1 },

				// Actual Examples
				// Basics
				{ "Scene", GS_EXAMPLE_SCENE, GS_EXAMPLE_SCENE_AVAILABLE, 0 },
				{ "Audio", GS_EXAMPLE_AUDIO, GS_EXAMPLE_AUDIO_AVAILABLE, 0 },
				{ "FMOD", GS_EXAMPLE_FMOD, GS_EXAMPLE_FMOD_AVAILABLE, 0 },
				{ "Free Flight Camera", GS_EXAMPLE_FREEFLIGHT, GS_EXAMPLE_FREEFLIGHT_AVAILABLE, 0 },
				{ "Model", GS_EXAMPLE_MODEL, GS_EXAMPLE_MODEL_AVAILABLE, 0 },
				{ "Material", GS_EXAMPLE_MATERIAL, GS_EXAMPLE_MATERIAL_AVAILABLE, 0 },
				{ "Textured", GS_EXAMPLE_TEXTURED, GS_EXAMPLE_TEXTURED_AVAILABLE, 0 },
				{ "Texture 2D", GS_EXAMPLE_TEXTURE2D, GS_EXAMPLE_TEXTURE2D_AVAILABLE, 0 },
				{ "Texture 3D", GS_EXAMPLE_TEXTURE3D, GS_EXAMPLE_TEXTURE3D_AVAILABLE, 0 },
				{ "Sprite", GS_EXAMPLE_SPRITE, GS_EXAMPLE_SPRITE_AVAILABLE, 0 },
				{ "Sprite System", GS_EXAMPLE_SPRITESYSTEM, GS_EXAMPLE_SPRITESYSTEM_AVAILABLE, 0 },
				{ "Cube Map", GS_EXAMPLE_CUBE_MAP, GS_EXAMPLE_CUBE_MAP_AVAILABLE, 0 },

				// Intermediate
				{ "Scene Shadowed", GS_EXAMPLE_SCENE_SHADOWS, GS_EXAMPLE_SCENE_SHADOWS_AVAILABLE, 1 },
				{ "Frame Buffer", GS_EXAMPLE_FRAMEBUFFER, GS_EXAMPLE_FRAMEBUFFER_AVAILABLE, 1 },
				{ "Deferred Scene", GS_EXAMPLE_DEFERREDSCENE, GS_EXAMPLE_DEFERREDSCENE_AVAILABLE, 1 },
				{ "Command Buckets", GS_EXAMPLE_COMMAND_BUCKET, GS_EXAMPLE_COMMAND_BUCKET_AVAILABLE, 1 },
				{ "Mesh Builder", GS_EXAMPLE_MESH_BUILDER, GS_EXAMPLE_MESH_BUILDER_AVAILABLE, 1 },
				{ "Particle System", GS_EXAMPLE_PARTICLESYSTEM, GS_EXAMPLE_PARTICLESYSTEM_AVAILABLE, 1 },
				{ "Spine", GS_EXAMPLE_SPINE, GS_EXAMPLE_SPINE_AVAILABLE, 1 },
				{ "IMGUI", GS_EXAMPLE_IMGUI, GS_EXAMPLE_IMGUI_AVAILABLE, 1 },
				{ "Ocornuts IMGUI", GS_EXAMPLE_OCORNUT_IMGUI, GS_EXAMPLE_OCORNUT_IMGUI_AVAILABLE, 1 },
				{ "Spherical Cube", GS_EXAMPLE_SPHERICALCUBE, GS_EXAMPLE_SPHERICALCUBE_AVAILABLE, 1 },
				{ "Mouse Intersect", GS_EXAMPLE_MOUSE_INTERSECT, GS_EXAMPLE_MOUSE_INTERSECT_AVAILABLE, 1 },

				// Advanced
				{ "Asio", GS_EXAMPLE_ASIO, GS_EXAMPLE_ASIO_AVAILABLE, 2 },
				{ "RakNet", GS_EXAMPLE_RAKNET, GS_EXAMPLE_RAKNET_AVAILABLE, 2 },
				{ "Physics", GS_EXAMPLE_PHYSICS, GS_EXAMPLE_PHYSICS_AVAILABLE, 2 },
				{ "Physics Character", GS_EXAMPLE_PHYSICSCHARACTER, GS_EXAMPLE_PHYSICSCHARACTER_AVAILABLE, 2 },
				{ "Skinning", GS_EXAMPLE_SKINNING, GS_EXAMPLE_SKINNING_AVAILABLE, 2 },
				{ "Shadows", GS_EXAMPLE_SHADOWS, GS_EXAMPLE_SHADOWS_AVAILABLE, 2 },
				{ "Scripting", GS_EXAMPLE_SCRIPTING, GS_EXAMPLE_SCRIPTING_AVAILABLE, 2 },
				{ "Lua", GS_EXAMPLE_LUA, GS_EXAMPLE_LUA_AVAILABLE, 2 },
				{ "Oculus", GS_EXAMPLE_OCULUS, GS_EXAMPLE_OCULUS_AVAILABLE, 2 },
				{ "OpenVR", GS_EXAMPLE_OPENVR2, GS_EXAMPLE_OPENVR2_AVAILABLE, 2 },
				{ "Server Client", GS_EXAMPLE_SERVER_CLIENT, GS_EXAMPLE_SERVER_CLIENT_AVAILABLE, 2 }
			};
			OPmemcpy(Examples, examples, sizeof(Example) * TotalEntries);

			CurrentHierarchy = -1;
		}

		// Copy the example names in an array for the Font Manager to use
		const OPchar* Names[TotalEntries];
		for (OPint i = 0; i < TotalEntries; i++) {
			Names[i] = Examples[i].name;
		}

		//FontManager = OPfontManagerSetup("Ubuntu.opf", Names, TotalEntries);
		//OPfont* font = (OPfont*)OPCMAN.LoadGet("Ubuntu.opf");
		FontManager = OPfontManager::Create("Ubuntu.opf", NULL, 0);
		FontManager->scale = 0.5;

		OPCMAN.Purge();
	}


	OPint Update(OPtimer* time) {

		if (OPGAMEPADS[0]->WasPressed(OPgamePadButton::Y)) {
			//OPGAMEPADS[0]->SetRumbleLong();
			OPGAMEPADS[0]->SetRumble(1, 1, 10000);
		}
		if (OPGAMEPADS[0]->WasPressed(OPgamePadButton::B)) {
			OPGAMEPADS[0]->SetRumbleShort();
		}

		// Get the number of available options in the current category
		i32 currentCategoryCount = 0;
		for (i32 i = 0; i < TotalEntries; i++) {
			if (Examples[i].parent != CurrentHierarchy) continue;
			currentCategoryCount++;
		}

		// Move the current menu selection up and down
		// Automatically wrap around if it exceeds the bounds of options
		//OPlogErr("%f, %f", SocketController->LeftThumbX(), SocketController->LeftThumbY());

		if (OPKEYBOARD.WasPressed(OPkeyboardKey::W) || OPKEYBOARD.WasPressed(OPkeyboardKey::UP) || Controller->LeftThumbNowUp() || Controller->WasPressed(OPgamePadButton::DPAD_UP)
#ifdef ADDON_socketio
			|| SocketController->LeftThumbNowUp() || SocketController->WasPressed(OPgamePadButton::DPAD_UP)
#endif
			) {
			Selected--;
		}
		if (OPKEYBOARD.WasPressed(OPkeyboardKey::S) || OPKEYBOARD.WasPressed(OPkeyboardKey::DOWN) || Controller->LeftThumbNowDown() || Controller->WasPressed(OPgamePadButton::DPAD_DOWN)
#ifdef ADDON_socketio
			|| SocketController->LeftThumbNowDown() || SocketController->WasPressed(OPgamePadButton::DPAD_DOWN)
#endif
			) {
			Selected++;
		}
		if (Selected < 0) Selected = currentCategoryCount - 1;
		if (Selected >= currentCategoryCount) Selected = 0;

		// Determine the actual selection within our entire array
		// Skips passed the category entries
		OPuint actualSelected = 0;
		for (i32 i = 0, j = 0; i < TotalEntries && j <= Selected; i++) {
			if (Examples[i].parent != CurrentHierarchy) continue;
			actualSelected = i;
			j++;
		}

		// When an example is selected:
		if (Examples[actualSelected].available && (OPKEYBOARD.WasPressed(OPkeyboardKey::SPACE) || OPKEYBOARD.WasPressed(OPkeyboardKey::E) || OPKEYBOARD.WasPressed(OPkeyboardKey::D) || OPKEYBOARD.WasPressed(OPkeyboardKey::ENTER) || Controller->WasPressed(OPgamePadButton::A) || Controller->WasPressed(OPgamePadButton::DPAD_RIGHT)
#ifdef ADDON_socketio
			|| SocketController->WasPressed(OPgamePadButton::A) || SocketController->WasPressed(OPgamePadButton::DPAD_RIGHT)
#endif
			)) {

			// Hard coded to category [3] which is Exit
			if (actualSelected == 3) {
				return 1;
			}

			// If the state is NULL then it's a category so we need to
			// delve further into the hierarchy
			if (Examples[actualSelected].state == NULL) {
				HierarchyDepth[CurrentHierarchy + 1] = actualSelected;
				CurrentHierarchy = actualSelected;
				CurrentDepth++;
				Selected = HierarchyDepth[CurrentHierarchy + 1];
			}
			else {
				// Otherwise it's an example, load it up.
				OPgameState::Change(Examples[actualSelected].state);
				return 0;
			}
		}

		// Jump backwards in the hierarchy
		if ((OPKEYBOARD.WasPressed(OPkeyboardKey::BACKSPACE) || OPKEYBOARD.WasPressed(OPkeyboardKey::A) || Controller->WasPressed(OPgamePadButton::BACK) || Controller->WasPressed(OPgamePadButton::B) || Controller->WasPressed(OPgamePadButton::DPAD_LEFT)
#ifdef ADDON_socketio
			|| SocketController->WasPressed(OPgamePadButton::B) || SocketController->WasPressed(OPgamePadButton::DPAD_LEFT)
#endif
			)) {
			HierarchyDepth[CurrentHierarchy + 1] = Selected;
			CurrentHierarchy = -1;
			CurrentDepth--;
			Selected = HierarchyDepth[CurrentHierarchy + 1];
		}

		return false;
	}

	void Render(OPfloat delta) {


		///////////////
		// RENDER
		///////////////

		OPrenderClear(0.4f, 0.1f, 0.1f);
		//
		// // Render the background
		OPtexture2DRender(Background);


		// Y coordinate to start drawing the text
		OPfloat start = -(Selected) * 40.0f + OPRENDERER_ACTIVE->OPWINDOW_ACTIVE->Height / 2.0f;


		OPfontRenderBegin(FontManager);

		OPfontColor(OPvec4(1.0, 1.0, 1.0, 1));
		FontManager->scale = 0.75;
		OPfontRender("OPengine v0.4.6", OPvec2(50, start - 60));

		FontManager->scale = 0.5;


		OPint notTheCurrentlySelectedMenuItem = 0, isActiveCategory = 0;
		f32 r, g, b;
		i32 pos = 0;
		for (i32 i = 0; i < TotalEntries; i++) {

			isActiveCategory = i == CurrentHierarchy;
			if (Examples[i].parent != CurrentHierarchy && !isActiveCategory) continue;

			notTheCurrentlySelectedMenuItem = Selected != pos;
			// Set Selected Color (bright yellow-ish gold)
			r = 0.95f, g = 0.84f; b = 0;

			if (notTheCurrentlySelectedMenuItem) {
				r = g = b = 1.0;
			}

			if (!Examples[i].available) {
				// Menu item is not available so make it really dark
				r = g = b = 0.3f;
				// Menu item is not available but it's the currently selected
				// item, so we'll brighten it just a bit so that we know what
				// is selected.
				if (!notTheCurrentlySelectedMenuItem) {
					r = g = b = 0.5;
				}
			}

			// If this is a category, then it becomes light blue
			if (isActiveCategory || Examples[i].state == NULL) {
				r = g = 0.7f; b = 1.0f;
				if (notTheCurrentlySelectedMenuItem && CurrentDepth == 0) {
					r = g = 0.4f; b = 0.7f;
				}
			}


			OPfontColor(OPvec4(r, g, b, 1));

			// If it's a category it doesn't get pushed to the right
			if (isActiveCategory) {
				OPfontRender(Examples[i].name,
					OPvec2(75, start + 40 * pos));
			}
			else {
				// If it's the root menu we don't offset to the right
				// If it isn't the root menu, then we push it to right
				// to help indicate that it's a sub-menu
				OPint isNotRootMenu = (CurrentHierarchy != -1) ? 1 : 0;
				OPfontRender(Examples[i].name,
					OPvec2((OPfloat)(75 + 40 * isNotRootMenu),
						start + 40 * (pos + isNotRootMenu)));
				pos++;
			}
		}

		OPfontRenderEnd();

		OPVISUALDEBUGINFO.Render(delta);

		OPrenderPresent();

		//OPlog("rendered example");
	}

	OPint Exit(OPgameState* next) {
		FontManager->Destroy();
		OPfree(FontManager);

		OPtexture2DDestroy(Background);
		OPtexture2DUnloadGlobals();
		OPfontSystemShutdownEffects();
		return 0;
	}

};

ExampleSelector _GS_EXAMPLE_SELECTOR;
OPgameState* GS_EXAMPLE_SELECTOR = &_GS_EXAMPLE_SELECTOR;