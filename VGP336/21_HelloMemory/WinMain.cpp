#include <3DEngine/Inc/3DEngine.h>

using namespace Engine3D;
using namespace Engine3D::Core;

static int sUniqueId = 0;

// Class Student
class Student
{
public:
    Student()
    {
        mId = ++sUniqueId;
        mMark = 60 + (rand() % 41);
        mName = " NoNmae_" + std::to_string(mId);
    }
    Student(const std::string& name)
        : mName(name)
    {
        mId = ++sUniqueId;
        mMark = 60 + (rand() % 41);
    }
    Student(const std::string& name, int mark)
        : mName(name)
        , mMark(mark)
    {
        mId = ++sUniqueId;
    }

    void Log()
    {
        LOG("Name: %s - ID: %d - Mark: %d", mName.c_str(), mId, mMark);
    }
private:
    int mId = 0;
    int mMark = 0;
    std::string mName;

};

// Class Dog
class Dog
{
public:
    Dog()
    {
        mId = ++sUniqueId;
        mBreed = "Unknown_" + std::to_string(mId);
        mName = "NoName_" + std::to_string(mId);
    }
    Dog(const std::string& name)
        : mName(name)
    {
        mId = ++sUniqueId;
        mBreed = "Unknown_" + std::to_string(mId);
    }
    Dog(const std::string& name, const std::string& breed)
        : mName(name)
        , mBreed(breed)
    {
        mId = ++sUniqueId;
    }

    void Log()
    {
        LOG("Name: %s - ID: %d - Breed: %s", mName.c_str(), mId, mBreed.c_str());
    }
private:
    int mId = 0;
    std::string mBreed;
    std::string mName;

};

// Class Car
class Car
{
public:
    Car()
    {
        mId = ++sUniqueId;
        mBrand = "Unknown_" + std::to_string(mId);
        mName = "NoName_" + std::to_string(mId);
    }
    Car(const std::string& name)
        : mName(name)
    {
        mId = ++sUniqueId;
        mBrand = "Unknown_" + std::to_string(mId);
    }
    Car(const std::string& name, const std::string& brand)
        : mName(name)
        , mBrand(brand)
    {
        mId = ++sUniqueId;
    }

    void Log()
    {
        LOG("Name: %s - ID: %d - Brand: %s", mName.c_str(), mId, mBrand.c_str());
    }
private:
    int mId = 0;
    std::string mBrand;
    std::string mName;

};

int WinMain(HINSTANCE instance, HINSTANCE, LPSTR, int)
{
    TypedAllocator studentPool = TypedAllocator<Student>("StudentPool", 100);
    TypedAllocator dogPool = TypedAllocator<Dog>("DogPool", 100);
    TypedAllocator carPool = TypedAllocator<Car>("CarPool", 100);

    std::vector<Student*> students;
    std::vector<Dog*> dogs;
    std::vector<Car*> cars;


    // test new student
    for (uint32_t i = 0; i < 70; ++i)
    {
        std::string newStudentName = " NewStudent" + std::to_string(i);
        Student* newstudent = studentPool.New(newStudentName, 75);
        newstudent->Log();
        students.push_back(newstudent);

    }

    // test new dog
    for (uint32_t i = 0; i < 70; ++i)
    {
        std::string newDogName = "NewDog" + std::to_string(i);
        Dog* newdog = dogPool.New(newDogName, "Labrador");
        newdog->Log();
        dogs.push_back(newdog);

    }

    // test new car
    for (uint32_t i = 0; i < 70; ++i)
    {
        std::string newCarName = "NewCar" + std::to_string(i);
        Car* newcar = carPool.New(newCarName, "Toyota");
        newcar->Log();
        cars.push_back(newcar);

    }

    // test delete student
    for (uint32_t i = 0; i < 40; ++i)
    {
        Student* student = students.back();
        studentPool.Delete(student);
        students.pop_back();
    }

    // test delete dog
    for (uint32_t i = 0; i < 40; ++i)
    {
        Dog* dog = dogs.back();
        dogPool.Delete(dog);
        dogs.pop_back();
    }

    // test delete car
    for (uint32_t i = 0; i < 40; ++i)
    {
        Car* car = cars.back();
        carPool.Delete(car);
        cars.pop_back();
    }

    // test add more student
    for (uint32_t i = 0; i < 70; ++i)
    {
        Student* newStudent = studentPool.New();
        newStudent->Log();
        students.push_back(newStudent);
    }

    // test add more dog
    for (uint32_t i = 0; i < 70; ++i)
    {
        Dog* newDog = dogPool.New();
        newDog->Log();
        dogs.push_back(newDog);
    }

    // test add more car
    for (uint32_t i = 0; i < 70; ++i)
    {
        Car* newCar = carPool.New();
        newCar->Log();
        cars.push_back(newCar);
    }

    // cleanup student
    for (Student* student : students)
    {
        student->Log();
        studentPool.Delete(student);
    }
    students.clear();

    // cleanup dog
    for (Dog* dog : dogs)
    {
        dog->Log();
        dogPool.Delete(dog);
    }
    dogs.clear();

    // cleanup car
    for (Car* car : cars)
    {
        car->Log();
        carPool.Delete(car);
    }
    cars.clear();

    return 0;

}