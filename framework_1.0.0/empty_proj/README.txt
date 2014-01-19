To set up a new project:


1. Open Xcode and create a new project and name it whatever you want (we'll call it newProj).

2. Copy over the source files from framework\empty_proj\Classes to newProj\Classes. Rename APPNAMEAppDelegate.h and APPNAMEAppDelegate.mm to newProjAppDelegate.h and newProjAppDelegate.mm, overwriting the automatically generated newProjAppDelegate files that were created by Xcode.

3. Go into the IDE and add the copied files to the Classes group.

4. In the IDE, create a new group called 'framework' and add the framework source files to it, from the framework\Classes folder. Make sure NOT to select "Copy items into destination group's folder" as the framework files are common across all projects.

5. (Optional) If needed, copy over the empty_proj\FlurryLib folder to the newProj folder. This is only needed if flurry is to be included in the application.

6. Copy over empty_proj\res to the newProj folder. This is where resource files will be placed. In the IDE, add the res\images and res\sounds folders to the 'Resources' group.

7. The following frameworks will need to be included for the new empty project to build: SystemConfiguration.framework, 
AudioToolBox.framework, 
OpenAL.framework, 
QuartzCore.framework, 
OpenGLES.framework, 
UIKit.framework, 
Foundation.framework, 
CoreGraphics.framework. 
Note that some may already be automatically included upon creation of the project.

8. Open MainWindow.xib and delete the 'Window' object. This is because we manually create our own rendering surface upon app delegate initialization.

9. Compile and run the build. A splash screen should be visible, and it should transition to a blank title screen.