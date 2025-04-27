#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <string>

using namespace std;

// Recipe class
class Recipe {
private:
    string name;
    string type; // Veg / Non-Veg
    vector<string> ingredients;
    string instructions;

public:
    Recipe() = default;
    Recipe(string n, string t, vector<string> ing, string instr)
        : name(n), type(t), ingredients(ing), instructions(instr) {}

    string getName() const { return name; }
    string getType() const { return type; }
    vector<string> getIngredients() const { return ingredients; }
    string getInstructions() const { return instructions; }

    void display() const {
        cout << "\n--- " << name << " (" << type << ") ---\n";
        cout << "Ingredients:\n";
        for (const auto& ing : ingredients) cout << "- " << ing << "\n";
        cout << "Instructions:\n" << instructions << "\n";
    }

    string toText() const {
        stringstream ss;
        ss << "Name: " << name << "\n";
        ss << "Type: " << type << "\n";
        ss << "Ingredients: ";
        for (size_t i = 0; i < ingredients.size(); ++i) {
            ss << ingredients[i];
            if (i != ingredients.size() - 1) ss << ", ";
        }
        ss << "\nInstructions: " << instructions << "\n---\n";
        return ss.str();
    }

    static Recipe fromTextBlock(const string& block) {
        stringstream ss(block);
        string line, name, type, ingredientsLine, instructions;
        vector<string> ingredients;

        while (getline(ss, line)) {
            if (line.rfind("Name:", 0) == 0) name = line.substr(6);
            else if (line.rfind("Type:", 0) == 0) type = line.substr(6);
            else if (line.rfind("Ingredients:", 0) == 0) {
                ingredientsLine = line.substr(13);
                stringstream ingSS(ingredientsLine);
                string ing;
                while (getline(ingSS, ing, ',')) {
                    ing.erase(0, ing.find_first_not_of(" "));
                    ingredients.push_back(ing);
                }
            }
            else if (line.rfind("Instructions:", 0) == 0) {
                instructions = line.substr(13);
            }
        }

        return Recipe(name, type, ingredients, instructions);
    }
};

// RecipeManager class
class RecipeManager {
private:
    vector<Recipe> recipes;
    string filename;

public:
    RecipeManager(const string& file) : filename(file) {
        loadRecipes();
    }

    void loadRecipes() {
        recipes.clear();
        ifstream file(filename);
        if (!file.is_open()) return;

        string line, block;
        while (getline(file, line)) {
            if (line == "---") {
                recipes.push_back(Recipe::fromTextBlock(block));
                block.clear();
            } else {
                block += line + "\n";
            }
        }
        file.close();
    }

    void saveRecipe(const Recipe& recipe) {
        ofstream file(filename, ios::app);
        file << recipe.toText();
        file.close();
    }

    void viewAllRecipes() const {
        if (recipes.empty()) {
            cout << "No recipes found.\n";
            return;
        }
        for (const auto& r : recipes) r.display();
    }

    void searchByIngredient(const string& keyword) const {
        bool found = false;
        for (const auto& r : recipes) {
            for (const auto& ing : r.getIngredients()) {
                if (ing.find(keyword) != string::npos) {
                    r.display();
                    found = true;
                    break;
                }
            }
        }
        if (!found) cout << "No recipe found with that ingredient.\n";
    }

    void filterByType(const string& pref) const {
        bool found = false;
        for (const auto& r : recipes) {
            if (r.getType() == pref) {
                r.display();
                found = true;
            }
        }
        if (!found) cout << "No " << pref << " recipes found.\n";
    }

    void addRecipe() {
        string name, type, input, instructions;
        vector<string> ingredients;

        cin.ignore();
        cout << "Enter recipe name: ";
        getline(cin, name);
        cout << "Enter type (Veg / Non-Veg): ";
        getline(cin, type);
        cout << "Enter ingredients (comma separated): ";
        getline(cin, input);
        stringstream ss(input);
        string item;
        while (getline(ss, item, ',')) {
            item.erase(0, item.find_first_not_of(" "));
            ingredients.push_back(item);
        }
        cout << "Enter instructions: ";
        getline(cin, instructions);

        Recipe r(name, type, ingredients, instructions);
        recipes.push_back(r);
        saveRecipe(r);

        cout << "Recipe added successfully!\n";
    }
};

// Main Function
int main() {
    RecipeManager manager("recipe.txt");
    int choice;
    string input;

    do {
        cout << "\nRecipe Generator Menu:\n";
        cout << "1. View All Recipes\n";
        cout << "2. Search by Ingredient\n";
        cout << "3. Filter by Veg/Non-Veg\n";
        cout << "4. Add New Recipe\n";
        cout << "5. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                manager.viewAllRecipes();
                break;
            case 2:
                cout << "Enter ingredient to search: ";
                cin >> input;
                manager.searchByIngredient(input);
                break;
            case 3:
                cout << "Enter preference (Veg/Non-Veg): ";
                cin >> input;
                manager.filterByType(input);
                break;
            case 4:
                manager.addRecipe();
                break;
            case 5:
                cout << "Exiting... Thank you!\n";
                break;
            default:
                cout << "Invalid choice.\n";
        }
    } while (choice != 5);

    return 0;
}
