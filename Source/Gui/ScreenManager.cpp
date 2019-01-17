#include "ScreenManager.h"
#include "Gui/Screens/IScreen.h"

ScreenManager::~ScreenManager(void)
{

}
ScreenManager::ScreenManager(void)
{

	//creates a stack for the screen
	ScreenManager::mScreens = std::vector<IScreen*>();
	ScreenManager::mDeadScreens = std::vector<IScreen*>();
}
void ScreenManager::PopScreen()
{

	IScreen* lScreen = ScreenManager::mScreens.back();
	ScreenManager::mScreens.pop_back();
	//delete the screen
	mDeadScreens.push_back(lScreen);
}
void ScreenManager::AddScreen(IScreen *Screen)
{
		//adds the screen to the screen manager
	ScreenManager::mScreens.push_back(Screen);
	Screen->Load(this);
}
void ScreenManager::RemoveScreen(IScreen* Screen)
{
	for(unsigned int x = 0; x < ScreenManager::mScreens.size(); x++)
	{
		if(ScreenManager::mScreens[x] == Screen)
		{
			ScreenManager::mScreens.erase(ScreenManager::mScreens.begin() +x);
			break;
		}
	}
	//screen is marked as dead
	mDeadScreens.push_back(Screen);

}
void ScreenManager::PopAllScreens()
{
	for(unsigned int x = 0; x < ScreenManager::mScreens.size(); x++)
	{
		mDeadScreens.push_back(ScreenManager::mScreens[x]);
	}
	ScreenManager::mScreens.clear();
}
void ScreenManager::Update(sf::RenderWindow* Window,float DeltaT)
{

	for( int x = ScreenManager::mScreens.size()-1; x >= 0; x--)
	{

		//updates the screen
		ScreenManager::mScreens[x]->Update(this,Window,DeltaT);

		//avaliable screens to clear
		if(ScreenManager::mDeadScreens.size() != 0)
		{
			//delete's any dead screens
			for(unsigned int x = 0; x < ScreenManager::mDeadScreens.size();x++)
			{
				delete ScreenManager::mDeadScreens[x];
			}
			ScreenManager::mDeadScreens.clear();

			break;
		}
		//this layer blocks the update loop from proceeding
		//good for pause screens
		if(ScreenManager::mScreens[x]->BlockUpdate())
		{
			break;
		}
		//layer can't be seen through so the function drops
		if(ScreenManager::mScreens[x]->Opaque())
		{
	
			//drops out of the loop if the screen is solid
			break;
		}
	}



}
void ScreenManager::Draw(sf::RenderWindow* Window)
{
	for(int x = ScreenManager::mScreens.size()-1; x >= 0; x--)
	{
		//reached an opaque layer and drops down to the secnd stage or the end of X
		if(ScreenManager::mScreens[x]->Opaque()  ||0 == x)
		{
			//has to draw the screens in revers order
			for(unsigned int y=x; y <=  ScreenManager::mScreens.size()-1; y++)
			{
				ScreenManager::mScreens[y]->Draw(Window);
			}
			//drops out of the loop if the screen is solid
			break;
		}
	}
}
