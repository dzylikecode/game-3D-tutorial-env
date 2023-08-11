// This file is part of the OGRE project.
// It is subject to the license terms in the LICENSE file found in the top-level
// directory of this distribution and at https://www.ogre3d.org/licensing.
// SPDX-License-Identifier: MIT

#include <Windows.h>
#include <stdlib.h>

#include "Ogre.h"
#include "OgreApplicationContext.h"
//! [key_handler]
class KeyHandler : public OgreBites::InputListener {
  bool keyPressed(const OgreBites::KeyboardEvent& evt) override {
    if (evt.keysym.sym == OgreBites::SDLK_ESCAPE) {
      Ogre::Root::getSingleton().queueEndRendering();
    }
    return true;
  }
};
//! [key_handler]
#define DYNLIB_LOAD(a) LoadLibraryEx(a, NULL, 0)
typedef struct HINSTANCE__* DYNLIB_HANDLE;
int main(int argc, char* argv[]) {
#ifdef MY_OGRE_CONFIG_DIR
  //! [constructor]
  auto config_dir = "OGRE_CONFIG_DIR=" MY_OGRE_CONFIG_DIR;
  putenv(config_dir);
  auto res = getenv("OGRE_CONFIG_DIR");
  auto fileName =
      "C:/Users/Z/Desktop/Dz/fishing/game-3D-tutorial-env/build/"
      "vcpkg_installed/x64-windows/debug/etc/ogre/../../plugins/"
      "ogre\\Plugin_DotScene_d.dll";
  // check if file exists
  FILE* file;
  file = fopen(fileName, "r");
  if (file == NULL) {
    printf("file not found\n");
    return 0;
  }
  auto s = (DYNLIB_HANDLE)DYNLIB_LOAD(fileName);
  auto err = GetLastError();
  printf("res: %p, err: %d\n", res, err);
#endif
  OgreBites::ApplicationContext ctx("OgreTutorialApp");
  ctx.initApp();
  //! [constructor]

  //! [setup]
  // get a pointer to the already created root
  Ogre::Root* root = ctx.getRoot();
  Ogre::SceneManager* scnMgr = root->createSceneManager();

  // register our scene with the RTSS
  Ogre::RTShader::ShaderGenerator* shadergen =
      Ogre::RTShader::ShaderGenerator::getSingletonPtr();
  shadergen->addSceneManager(scnMgr);

  // without light we would just get a black screen
  Ogre::Light* light = scnMgr->createLight("MainLight");
  Ogre::SceneNode* lightNode =
      scnMgr->getRootSceneNode()->createChildSceneNode();
  lightNode->setPosition(0, 10, 15);
  lightNode->attachObject(light);

  // also need to tell where we are
  Ogre::SceneNode* camNode = scnMgr->getRootSceneNode()->createChildSceneNode();
  camNode->setPosition(0, 0, 15);
  camNode->lookAt(Ogre::Vector3(0, 0, -1), Ogre::Node::TS_PARENT);

  // create the camera
  Ogre::Camera* cam = scnMgr->createCamera("myCam");
  cam->setNearClipDistance(5);  // specific to this sample
  cam->setAutoAspectRatio(true);
  camNode->attachObject(cam);

  // and tell it to render into the main window
  ctx.getRenderWindow()->addViewport(cam);

  // finally something to render
  Ogre::Entity* ent = scnMgr->createEntity("Sinbad.mesh");
  Ogre::SceneNode* node = scnMgr->getRootSceneNode()->createChildSceneNode();
  node->attachObject(ent);
  //! [setup]

  //! [main]
  // register for input events
  KeyHandler keyHandler;
  ctx.addInputListener(&keyHandler);

  ctx.getRoot()->startRendering();
  ctx.closeApp();
  //! [main]
  return 0;
}