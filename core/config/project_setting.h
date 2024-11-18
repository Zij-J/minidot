// project settings (singleton for "dynamic reload & update settings from files", which isn't implemented in this codebase, yet.)

#ifndef PROJECT_SETTING_H
#define PROJECT_SETTING_H


class ProjectSetting {
public:
    const char *window_name = "Tank Fight!";
    int window_width = 960; // width/height = 16/9
    int window_height;

    const int design_width = 1920; 
    const int design_height; 


    static ProjectSetting &get_singleton() {
        if (singleton == nullptr) { // lazy init
            singleton = new ProjectSetting();
        }
        return *singleton;
    }
    
    ~ProjectSetting() {
        if (!is_deleting) {
            is_deleting = true;
            delete singleton; 
        }
    }


private:
    static ProjectSetting *singleton;   // for "reload", so it must deletable outside class, so no local static varible implement 
    static bool is_deleting;

    ProjectSetting(): 
    window_height(window_width / 16 * 9),
    design_height(design_width / 16 * 9) {}
};

#endif