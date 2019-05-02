//Making the path name dynamicaly with each new varible name.
    std::ostringstream oss;
    oss << framePath << fileName << q << ".txt";
    std::string path1 = oss.str();
    cout << "This is the First Path: " << path1 << endl;

    //Making the path name dynamicaly with each new varible name.(Second Compare File)
    std::ostringstream oss1;
    oss1 << framePath << fileName << (q + 1) << ".txt";
    std::string path2 = oss1.str();
    cout << "This is the second path: " << path2 << endl;


    string line, field;
    int i = 0;
    int x = 0;
    static const int num_threads = 5;
    bool movement1 = false;
    bool movement2 = false;
    vector<int> frames0;
    vector<int> frames1;
    bool sizeCheck = false;

    frames0 = txtRead(path1);
    frames1 = txtRead(path2);

    if(frames0.size()< frames1.size())
    {
        sizeCheck = false;
    }
    else if (frames1.size() < frames0.size())
    {
        sizeCheck = true;
    }
    

    cout << frames0[0] << endl;

    thread check[num_threads];

    for(int q = 0; q < num_threads; q++)
    {
        std::ostringstream oss;
        oss << framePath << fileName << q << ".txt";
        std::string path1 = oss.str();
        cout << "This is the First Path: " << path1 << endl;
    
        std::ostringstream oss1;
        oss1 << framePath << fileName << (q + 1) << ".txt";
        std::string path2 = oss1.str();
        cout << "This is the second path: " << path2 << endl;
        
        frames0 = txtRead(path1);
        frames1 = txtRead(path2);

        if(frames0.size()< frames1.size())
        {
            sizeCheck = false;
        }
        else if (frames1.size() < frames0.size())
        {
            sizeCheck = true;
        }
    


        check[q] = std::thread(diffCheck,std::ref(frames0) ,std::ref(frames1),std::ref(movement1),threshAmount,sizeCheck);
    }
    for(int z = 0; z < num_threads; z++)
    {
        check[z].join();
    }