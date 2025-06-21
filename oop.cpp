#include <iostream>
#include <vector>
#include <string>
#include <chrono>

enum class AssetType { ITEM, DOCUMENT };
enum class ItemType { LAPTOP, ID_CARD };
enum class DocumentType { FINANCE, IT, HR };

// base class
class Asset {
protected:
    std::string name;
    std::string assignee;
public:
    Asset(const std::string& name, const std::string& assignee)
        : name(name), assignee(assignee) {}
    virtual ~Asset() = default;

    virtual void display(size_t index) const = 0;
};

// derived class for Items
class ItemAsset : public Asset {
    ItemType item_type;
public:
    ItemAsset(ItemType type, const std::string& name, const std::string& assignee)
        : Asset(name, assignee), item_type(type) {}

    void display(size_t index) const override {
        std::string type_str = (item_type == ItemType::LAPTOP) ? "Laptop" : "ID Card";
        std::cout << "Asset " << index + 1 << ": Type: Item, Item: " << type_str
                  << ", Name: " << name << ", Assignee: " << assignee << "\n";
    }
};

// derived class for Documents
class DocumentAsset : public Asset {
    DocumentType document_type;
public:
    DocumentAsset(DocumentType type, const std::string& name, const std::string& assignee)
        : Asset(name, assignee), document_type(type) {}

    void display(size_t index) const override {
        std::string type_str;
        switch (document_type) {
            case DocumentType::FINANCE: type_str = "Finance"; break;
            case DocumentType::IT:      type_str = "IT"; break;
            case DocumentType::HR:      type_str = "HR"; break;
            default:                    type_str = "Unknown"; break;
        }
        std::cout << "Asset " << index + 1 << ": Type: Document, Division: " << type_str
                  << ", Name: " << name << ", Assignee: " << assignee << "\n";
    }
};

// asset manager (like AssetList)
class AssetManager {
    std::vector<Asset*> assets;
public:
    ~AssetManager() {
        for (auto asset : assets)
            delete asset;
    }

    void add(Asset* asset) {
        assets.push_back(asset);
    }

    void remove(size_t index) {
        if (index < assets.size()) {
            delete assets[index];
            assets.erase(assets.begin() + index);
        }
    }

    void display() const {
        for (size_t i = 0; i < assets.size(); ++i)
            assets[i]->display(i);
    }
};

// main program
int main() {
    auto start = std::chrono::high_resolution_clock::now();

    AssetManager manager;
    manager.add(new ItemAsset(ItemType::LAPTOP, "Dell XPS 13", "Alice"));
    manager.add(new ItemAsset(ItemType::ID_CARD, "ID12345", "Bob"));
    manager.add(new DocumentAsset(DocumentType::IT, "IT Policy", "Charlie"));

    std::cout << "Initial asset list:\n";
    manager.display();

    std::cout << "\nRemoving second asset...\n";
    manager.remove(1);

    std::cout << "\nUpdated asset list:\n";
    manager.display();

    auto end = std::chrono::high_resolution_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    std::cout << "\nElapsed time: " << elapsed << " microseconds\n";

    return 0;
}
