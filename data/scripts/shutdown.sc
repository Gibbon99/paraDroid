
//-----------------------------------------------------------------------------
//
// Run any commands from the script before shutting down
void as_shutdown()
//-----------------------------------------------------------------------------
{
	io_saveConfigValueInt("useCollisionDetection", useCollisionDetection == true ? 1 : 0);
	io_saveConfigValueInt("playSounds", playSounds == true ? 1 : 0);
	io_saveConfigValueInt("useHoldBitmap", useHoldBitmap == true ? 1 : 0);
	io_saveConfigValueInt("useBackingBitmap", useBackingBitmap == true ? 1 : 0);
	io_saveConfigValueInt("vsyncOption", vsyncOption);
	io_saveConfigValueInt("swapMethod", swapMethod);
	io_saveConfigValueInt("fullScreen", fullScreenValue);
	io_saveConfigValueString("tileColor", currentTileColorStr);
	io_saveConfigValueString("tileType", currentTileTypeStr);
	io_saveConfigValueString("volumeLevel", volumeLevelStr);
	io_saveConfigValueInt("particleType", particleRenderType);
	io_saveConfigValueInt("language", currentLanguage);
	
//	io_saveConfigValueFloat("progressBarLoadTimes", progressBarLoadTimes);
//	io_saveConfigValueString("allDroidsVisible", allDroidsVisible == true ? "1" : "0");

	io_saveConfigFile("config.ini");
	io_saveKeyBindings("keybind.dat");

	io_closeConfigFile();
}
