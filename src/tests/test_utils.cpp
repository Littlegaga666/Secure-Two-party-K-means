/**
 * @author: Jamie
 * @brief: Testing cases for utils library
 */
 #include <utils/utils.h>
#include <sys/timeb.h>
#include <utils/encoder.h>
#include <utils/data.h>

using namespace bioutils;

void test_encoder(){
    std::cout << "*******************************\n";
    std::cout << "*        ENCODER TEST         *\n";
    std::cout << "*******************************\n";

    GeneralEncoder* encoder = new GeneralEncoder;
    struct timeb start, finish;

    ftime(&start);
    encoder->Preprocess("../../data/S_A.txt", "../../data/S_A_ph_test.txt", "test.txt");
    ftime(&finish);
    std::cout << ">> Preprocessing S Dataset: " << ((((double)finish.time - (double)start.time) * (double)1000 +
        ((double)finish.millitm - (double)start.millitm)) / (double)1000) << " (s) " << std::endl;

    ftime(&start);
    encoder->Preprocess("../../data/M_A.txt", "../../data/M_A_ph.txt", "test.txt");
    ftime(&finish);
    std::cout << ">> Preprocessing M Dataset: " << ((((double)finish.time - (double)start.time) * (double)1000 +
       ((double)finish.millitm - (double)start.millitm)) / (double)1000) << " (s) " << std::endl;

    ftime(&start);
    encoder->Preprocess("../../data/M2_A.txt", "../../data/M2_A_ph.txt", "test.txt");
    ftime(&finish);
    std::cout << ">> Preprocessing M2 Dataset: " << ((((double)finish.time - (double)start.time) * (double)1000 +
        ((double)finish.millitm - (double)start.millitm)) / (double)1000) << " (s) " << std::endl;

    ftime(&start);
    encoder->Preprocess("../../data/L_A.txt", "../../data/L_A_ph.txt", "test.txt");
    ftime(&finish);
    std::cout << ">> Preprocessing L Dataset: " << ((((double)finish.time - (double)start.time) * (double)1000 +
       ((double)finish.millitm - (double)start.millitm)) / (double)1000) << " (s) " << std::endl;

    ftime(&start);
    std::vector<std::string> files;
    files.push_back("../../data/S_A_ph.txt");
    files.push_back("../../data/S_A_ph_test.txt");
    encoder->Preprocess("../../data/S_A.txt", files, "test.txt");
    ftime(&finish);
    std::cout << ">> Preprocessing S multi traits Dataset: " << ((((double)finish.time - (double)start.time) * (double)1000 +
                     ((double)finish.millitm - (double)start.millitm)) / (double)1000) << " (s) " << std::endl;

    std::remove("test.txt");
    std::cout << std::endl;
    delete encoder;
}

void test_data(){
    std::cout << "*******************************\n";
    std::cout << "*        DATA TEST            *\n";
    std::cout << "*******************************\n";
    struct timeb start, finish;
    GeneralData* data = new GeneralData();
    GeneralEncoder* encoder = new GeneralEncoder;

    /* Please note in this test phenotype file has more than 1 traits */
    encoder->Preprocess("../../data/S_A.txt", "../../data/S_A_ph_test.txt", "test.txt");

    ftime(&start);
    data->ReadData("test.txt"); // read from file
    ftime(&finish);
    std::cout << ">> data->ReadData(): " << ((((double)finish.time - (double)start.time) * (double)1000 +
        ((double)finish.millitm - (double)start.millitm)) / (double)1000) << " (s) " << std::endl;

    ftime(&start);
    data->SaveData("test_write.txt");
    ftime(&finish);
    std::cout << ">> data->SaveData(): " << ((((double)finish.time - (double)start.time) * (double)1000 +
        ((double)finish.millitm - (double)start.millitm)) / (double)1000) << " (s) " << std::endl;

    std::remove("test.txt");
    std::remove("test_write.txt");

    data->Print();
    data->RemoveFeature(13);
    data->Print();
    std::cout << std::endl;
    delete encoder;
    delete data;
}

void test_wvector(){
    std::cout << "*******************************\n";
    std::cout << "*        WVECTOR TEST         *\n";
    std::cout << "*******************************\n";
    WVector<int>* working_vec = new WVector<int>(3, 2);
    for(int i=0; i<6; i++)
        working_vec->GetRawPtr()[i] = i;

    working_vec->Print();
    working_vec->Transpose();
    working_vec->Print();
    working_vec->RemoveCol(2); // remove 3rd column;
    working_vec->Print();
    working_vec->RemoveRow(0); // remove 1st row;
    working_vec->Print();

    std::cout << std::endl;
    delete(working_vec);
}

int main(){
    test_encoder();
    test_data();
    test_wvector();
    return 0;
}
