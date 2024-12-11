#include <iostream>
#include <chrono>
#include <map>

class Product {
    std::string name;
    unsigned int id;
    float weight;
    float price;
    unsigned int storage_life;
public:
    Product() {
        name = "";
        id = 0;
        weight = 0;
        price = 0;
        storage_life = 0;
    }
    Product(std::string Name, unsigned int ID, float Weight, float Price, unsigned int Time) {
        name = Name;
        id = ID;
        weight = Weight;
        price = Price;
        storage_life = Time;
    }
    Product(const Product& old) {
        name = old.name;
        id = old.id;
        weight = old.weight;
        price = old.price;
        storage_life = old.storage_life;
    }

    virtual ~Product () = default;

    virtual float calculateStorageFee() {
        float cost = weight;
        return cost;
    }

    unsigned int getId() const {
        return id;
    }

    virtual void displayInfo() {
        std::cout << "Info about product " << name << ":\n";
        std::cout << "ID: " << id << '\n';
        std::cout << "Weight: " << weight << '\n';
        std::cout << "Price: " << price << '\n';
        std::cout << "Storage life: " << storage_life << '\n';
    }

    Product& operator= (const Product& a) = default;
};

class PerishableProduct : public Product {
    time_t expireDate;
public:
    PerishableProduct() {
        expireDate = 0;
    };
    PerishableProduct(std::string Name, unsigned int ID, float Weight, float Price, unsigned int Time, time_t end) : Product(Name, ID, Weight, Price, Time) {
        expireDate = end;
    };
    PerishableProduct(PerishableProduct& old)  : Product(old) {
        expireDate = old.expireDate;
    }
    ~PerishableProduct () override = default;

    PerishableProduct& operator= (const PerishableProduct& a) {
        Product::operator= (a);
        expireDate = a.expireDate;
        return *this;
    }

    float calculateStorageFee() override {
        float cost = Product::calculateStorageFee();
        time_t now = time(nullptr);
        float kef = (float)(expireDate - now) / 3600 / 24;
        if (kef > 0) {
            cost /= kef;
        } else {
            cost *= kef;
        }
        std::cout << (float)(expireDate - now) / 3600 / 24;
        return cost;
    }

    double deadline() const {
        time_t now = time(nullptr);
        double ans = (double)(expireDate - now) / 3600 / 24;
        return ans;
    }
};

class ElectronicProduct : public Product {
    unsigned int warrantyPeriod;
    float powerRating;
public:
    ElectronicProduct() {
        warrantyPeriod = 0;
        powerRating = 0;
    };
    ElectronicProduct(std::string Name, unsigned int ID, float Weight, float Price, unsigned int Time,
                      unsigned int end, float power) : Product(Name, ID, Weight, Price, Time) {
        warrantyPeriod = end;
        powerRating = power;
    };
    ElectronicProduct(ElectronicProduct& old)  : Product(old) {
        warrantyPeriod = old.warrantyPeriod;
        powerRating = old.powerRating;
    }
    ~ElectronicProduct () override = default;

    ElectronicProduct& operator= (const ElectronicProduct& a) {
        Product::operator= (a);
        warrantyPeriod = a.warrantyPeriod;
        powerRating = a.powerRating;
        return *this;
    }

    void displayInfo() override {
        Product::displayInfo();
        std::cout << "Warranty period: " << warrantyPeriod << '\n';
        std::cout << "Power rating: " << powerRating << '\n';
    }
};

class BuildingMaterial : public Product {
    bool flammability;
public:
    BuildingMaterial() {
        flammability = false;
    };
    BuildingMaterial(std::string Title, unsigned int ID, float Weight, float Price, unsigned int Time, bool fireDanger) : Product(Title, ID, Weight, Price, Time) {
        flammability = fireDanger;
    };
    BuildingMaterial(BuildingMaterial& old)  : Product(old) {
        flammability = old.flammability;
    }
    ~BuildingMaterial () override = default;

    BuildingMaterial& operator= (const BuildingMaterial& a) {
        Product::operator= (a);
        flammability = a.flammability;
        return *this;
    }

    float calculateStorageFee() override {
        float cost = Product::calculateStorageFee();
        if (flammability) {
            cost *= 2;
        }
        return cost;
    }
};

class Warehouse  {
    std::vector<std::shared_ptr<Product>> array;
public:
    void addProduct(const std::shared_ptr<Product>& a) {
        array.push_back(a);
    }

    Warehouse& operator+= (const std::shared_ptr<Product>& a) {
        addProduct(a);
        return *this;
    }

    void popProduct(unsigned int id) {
        int size = (int)array.size();
        for (int i = 0; i < size; ++i) {
            if (array[i]->getId() == id) {
                array.erase(array.begin() + i);
                break;
            }
        }
    }

    Warehouse& operator-= (unsigned int id) {
        popProduct(id);
        return *this;
    }

    std::shared_ptr<Product> findProduct(unsigned int id) {
        for (const auto& el : array) {
            if (el->getId() == id) {
                return el;
            }
        }
        return nullptr;
    }

    std::shared_ptr<Product> operator[] (unsigned int id) {
        return findProduct(id);
    }

    std::vector<std::shared_ptr<Product>> getExpiringProducts(unsigned int days) {
        std::vector<std::shared_ptr<Product>> dead;
        for (const auto& el : array) {
            std::shared_ptr<PerishableProduct> product = std::static_pointer_cast<PerishableProduct>(el);
            if (product && (product->deadline() <= days)) {
                dead.push_back(product);
            }
        }
        return dead;
    }

    double calculateWarehouseFee() const {
        double total = 0;
        for (const auto& el : array) {
            total += el->calculateStorageFee();
        }
        return total;
    }

    void displayInventory() const {
        std::map<std::string, std::vector<std::shared_ptr<Product>>> sorted_arr;
        for (const auto& el : array) {
            if (auto perishable = std::dynamic_pointer_cast<PerishableProduct>(el)) {
                sorted_arr["Perishable Products"].push_back(perishable);
            } else if (auto electronic = std::dynamic_pointer_cast<ElectronicProduct>(el)) {
                sorted_arr["Electronic Products"].push_back(electronic);
            } else if (auto buildingMaterial = std::dynamic_pointer_cast<BuildingMaterial>(el)) {
                sorted_arr["Building Material"].push_back(buildingMaterial);
            } else {
                sorted_arr["Product"].push_back(el);
            }
        }

        for (const auto& [category, products] : sorted_arr) {
            std::cout << category << ":\n";
            for (const auto& product : products) {
                product->displayInfo();
            }
            std::cout << '\n';
        }
    }

    friend std::ostream& operator << (std::ostream &out, const Warehouse& sklad) {
        out << "Warehouse:\n";
        sklad.displayInventory();
        out << "Warehouse value: " << sklad.calculateWarehouseFee() << '\n';
        return out;
    }
};


int main() {
    time_t now = time(nullptr);
    PerishableProduct a = PerishableProduct{"banana", 2, 1.2, 120, 5, now + 14 * 3600 * 24};
    ElectronicProduct b = ElectronicProduct{"rozetka", 3, 0.2, 50, 5, 30,220};
    BuildingMaterial c = BuildingMaterial{"wood", 5, 12, 10, 13, true};

    Warehouse sklad;
    sklad += std::make_shared<PerishableProduct>(a);
    sklad += std::make_shared<ElectronicProduct>(b);
    sklad += std::make_shared<BuildingMaterial>(c);
    std::cout << sklad;
    std::cout << "////////////////\n";
    sklad -= 5;
    sklad[2]->displayInfo();
    std::cout << sklad;
    sklad.getExpiringProducts(15);
}