#ifndef APP_MANAGER_H
#define APP_MANAGER_H

struct Params {
    int  speed  = 15;
    int  length = 15;
    bool epilepsy = false;
};

class AppManager {
public:
    explicit AppManager(const Params& params);
    void run();

private:
    Params params_;
};

#endif
