class Counter {
    public:
        int release(){
            return --m_RefCount;
        }

        int add_ref(){
            return ++m_RefCount;
        }

        int refCounter(){
            return m_RefCount;
        }
    private:
        int m_RefCount = 0;
};

template <typename T>
class SmartPtrCustom {
    public:
        SmartPtrCustom() : m_Data(nullptr), m_Counter(nullptr) {
            m_Counter = new Counter();
        }

        ~SmartPtrCustom(){
            if(m_Counter->release() == 0){
                delete m_Data;
            }
        }

        SmartPtrCustom(T* a_Data) : m_Data(a_Data), m_Counter(nullptr) {
            m_Counter = new Counter();
            m_Counter->add_ref();
        }

        SmartPtrCustom<T>& operator=(const SmartPtrCustom<T>& rhs){
            if(rhs == *this){
                if(m_Counter->release() == 0){
                    delete m_Data;
                }

                m_Counter = rhs.m_Counter;
                m_Counter->add_ref();

                m_Data = rhs.m_Data;
            }

            return *this;
        }

        SmartPtrCustom(const SmartPtrCustom<T>& src) : m_Data(src.m_Data), m_Counter(src.m_Counter) {
            m_Counter->add_ref();
        }

        T operator*(){
            return *m_Data;
        }

        T* operator->(){
            return m_Data;
        }

        int getRefCounter(){
            return m_Counter->refCounter();
        }


    private:
        T* m_Data;
        Counter* m_Counter;
};

class Person {
    public:
        int age;
        int type;
};

int main(){
    auto p = new Person {1,34};
    SmartPtrCustom<Person> ptr(p);
    {
        cout << ptr->age << "-" << ptr->type << endl;
        cout << ptr.getRefCounter() << endl;;
        {
            SmartPtrCustom<Person> ptr1(ptr);
            cout << ptr1.getRefCounter() << "-" << ptr.getRefCounter() << endl;
        }

        cout << ptr.getRefCounter() << endl;

    }


    return 0;
}
