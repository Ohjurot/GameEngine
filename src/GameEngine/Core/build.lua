mox_project("Core", "gameengine_core")
mox_cpp("C++20")
mox_staticlib()

dependson { 
    "imgui", 
}
