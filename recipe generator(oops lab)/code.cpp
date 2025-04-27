#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
using namespace std;

class Recipe
{
    string recipe_name;
    string category;
    vector<string> ingredients;
    vector<string> instructions;

public:
    // constructors
    Recipe() : recipe_name(""), category(), ingredients(), instructions() {};

    // getter functions
    string get_recipe_name() { return recipe_name; }
    string get_category() { return category; }
    vector<string> get_recipe_ingredients() { return ingredients; }
    vector<string> get_recipe_instructions() { return instructions; }

    // setter function
    void set_recipe_name(string name) { recipe_name = name; }
    void set_category(string cat) { category = cat; }
    void set_recipe_ingredients(const string &ing) { ingredients.push_back(ing); }
    void set_recipe_instructions(const string &instr) { instructions.push_back(instr); }

    // display function to display recipe(not menu)
    void display()
    {
        cout << "Recipe Name: " << recipe_name << endl;
        cout << "Category: " << category << endl;
        cout << "Ingredients: " << endl;
        for (auto i : ingredients)
        {
            cout << i << ",";
        }
        cout << endl;
        cout << "Instructions: " << endl;
        for (auto i : instructions)
        {
            cout << i << endl;
        }
    }

    Recipe convert_string_to_recipe(string recipe_textblock)
    {
        Recipe r;
        stringstream object(recipe_textblock);
        string line, ingredient_line, instruction_line;
        vector<string> ingredients, instructions;
        while (getline(object, line))
        {
            if (line.rfind("Recipe Name: ", 0) == 0)
            {
                r.recipe_name = line.substr(13);
            }
            else if (line.rfind("Category: ", 0) == 0)
            {
                r.category = line.substr(10);
            }
            else if (line.rfind("Ingredients: ", 0) == 0)
            {
                ingredient_line = line.substr(13);
                stringstream ingredient_item(ingredient_line);
                string item;
                while (getline(ingredient_item, item, ','))
                {
                    item.erase(0, item.find_first_not_of(" "));
                    r.ingredients.push_back(item);
                }
            }
            else if (line.rfind("Instructions: ", 0) == 0)
            {
                while ((line.rfind("---", 0) == string::npos) && getline(object, line))
                {
                    r.instructions.push_back(line);
                }
            }
        }
        return r;
    }

    string convert_obj_sstring(const Recipe r)
    {
        stringstream recipe_file;
        recipe_file << "Recipe Name: " << r.recipe_name << endl;
        recipe_file << "Category: " << r.category << endl;
        recipe_file << "Ingredients: ";
        for (auto i : r.ingredients)
        {
            recipe_file << i << ",";
        }
        recipe_file << endl;
        recipe_file << "Instructions: " << endl;
        for (auto i : r.instructions)
        {
            recipe_file << i << endl;
        }
        return recipe_file.str();
    }
};

class Recipe_Manager
{
    Recipe r;
    vector<Recipe> recipes;

public:
    Recipe_Manager(string filename)
    {
        loadall_Recipes(filename);
    }

    void loadall_Recipes(string filename)
    {
        ifstream file(filename);
        if (!file.is_open())
        {
            return;
        }
        stringstream recipe_textblock;
        string line;
        while (getline(file, line))
        {
            if (line.rfind("Recipe Name: ", 0) == 0)
            {
                recipe_textblock.str("");
                recipe_textblock << line << endl;
            }
            else if (line.rfind("---", 0) == 0)
            {
                recipes.push_back(r.convert_string_to_recipe(recipe_textblock.str()));
                recipe_textblock.str("");
                recipe_textblock.clear();
            }
            else
            {
                recipe_textblock << line << endl;
            }
        }
    }

    void display_menu()
    {
        for (auto r : recipes)
        {
            r.display();
            cout << "------------------------" << endl;
        }
    }

    Recipe search_by_name(string name)
    {
        for (auto r : recipes)
        {
            if (r.get_recipe_name() == name)
            {
                return r;
            }
        }
        cout << "Recipe not found with that name." << endl;
        return Recipe();
    }

    Recipe search_by_ingredients(const vector<string> &input_ingredients)
    {
        int max_matches = 0;
        Recipe best_match;
        for (auto &r : recipes)
        {
            int match_count = 0;
            vector<string> recipe_ingredients = r.get_recipe_ingredients();
            for (const auto &ing : input_ingredients)
            {
                for (const auto &recipe_ing : recipe_ingredients)
                {
                    if (ing == recipe_ing)
                    {
                        match_count++;
                        break;
                    }
                }
            }
            if (match_count > max_matches)
            {
                max_matches = match_count;
                best_match = r;
            }
        }
        if (max_matches == 0)
        {
            cout << "No matching recipes found." << endl;
            return Recipe();
        }
        else
        {
            return best_match;
        }
    }

    Recipe search_by_category(string category)
    {
        for (auto r : recipes)
        {
            if (r.get_category() == category)
            {
                return r;
            }
        }
        cout << "Recipe not found with that category." << endl;
        return Recipe();
    }
    void add_recipe(Recipe r)
    {
        ofstream file("recipe.txt", ios::app);
        if (!file.is_open())
        {
            cout << "Error opening file." << endl;
            return;
        }
        string recipe_text = r.convert_obj_sstring(r);
        file << recipe_text << "---" << endl;

        file.close();
    }
    void add_favorite(Recipe r)
    {
        ofstream file("favorite.txt", ios::app);
        if (!file.is_open())
        {
            cout << "Error opening file." << endl;
            return;
        }
        string recipe_text = r.convert_obj_sstring(r);
        file << recipe_text << "---" << endl;
        file.close();
    }
};

int main()
{
    cout << "\n-----------------Welcome to a Recipe Generator-------------------\n";
    int choice;
    string filename;
    cout << "This is a program where you can find recipes(available ingredients / by their name).\n";
    cout << "\nDo you want recipes from your favorites or from recipe book.\n";
    string filen;
    getline(cin, filen);
    if (filen == "Book" || filen == "book")
    {
        filename = "recipe.txt";
    }
    else if (filen == "Favorite" || filen == "favorite")
    {
        filename = "favorite.txt";
    }
    else
    {
        return 0;
    }
    Recipe_Manager manager(filename);
    cout << "1. View Menu\n2. Search by Name\n3. Search by Ingredients\n4. Add Recipe\n5. Exit\n";
    cout << "--------------------------------------------------------\n";
    cout << "Please select an option from the menu above.\n\n";
    cout << "Enter your choice: ";
    cin >> choice;
    while (choice != 5)
    {
        switch (choice)
        {
        case 1:
            cout << "\n-----------------Menu-------------------\n";
            manager.display_menu();
            break;
        case 2:
        {
            string name;
            cin.ignore();
            cout << "Enter recipe name: ";
            cout << endl;
            getline(cin, name);
            cout << "----------------------------------------\n";
            Recipe r = manager.search_by_name(name);
            if (!r.get_recipe_name().empty())
            {
                r.display();
                cout << "----------------------------------------\n";
                cout << "Would you like to add this recipe to your favorites? (y/n): ";
                char add_fav;
                cin >> add_fav;
                if (add_fav == 'y' || add_fav == 'Y')
                {
                    Recipe r = manager.search_by_name(name);
                    manager.add_favorite(r);
                    cout << "Recipe added to favorites!" << endl;
                }
            }
            cout << "---------------------------------------------\n";
            break;
        }
        case 3:
        {
            string input;
            cout << "Enter ingredients (comma separated): ";
            cout << endl;
            cin.ignore();
            getline(cin, input);
            cout << "--------------------(Recipes that best matches your ingredients)--------------------\n\n";
            stringstream ss(input);
            string item;
            vector<string> ingredients;
            while (getline(ss, item, ','))
            {
                item.erase(0, item.find_first_not_of(" "));
                ingredients.push_back(item);
            }
            Recipe r = manager.search_by_ingredients(ingredients);
            if (!r.get_recipe_name().empty())
            {
                r.display();
                cout << "-------------------------------------------\n\n";
                cout << "Would you like to add this recipe to your favorites? (y/n): ";
                char add_fav;
                cin >> add_fav;
                if (add_fav == 'y' || add_fav == 'Y')
                {
                    manager.add_favorite(r);
                    cout << "Recipe added to favorites!" << endl;
                }
            }
            cout << "------------------------------------------------\n\n";
            break;
        }
        case 4:
        {
            Recipe new_recipe;
            string name, ingredient, instruction;
            cin.ignore();
            cout << "Enter recipe name: ";
            getline(cin, name);
            new_recipe.set_recipe_name(name);
            cout << "Enter category: ";
            string category;
            getline(cin, category);
            new_recipe.set_category(category);
            cout << "Enter ingredients (comma separated): ";
            getline(cin, ingredient);
            stringstream ss(ingredient);
            string item;
            while (getline(ss, item, ','))
            {
                item.erase(0, item.find_first_not_of(" "));
                new_recipe.set_recipe_ingredients(item);
            }
            cout << "Enter instructions (end with 'END'): " << endl;
            while (true)
            {
                getline(cin, instruction);
                if (instruction == "END")
                {
                    break;
                }
                new_recipe.set_recipe_instructions(instruction);
            }
            manager.add_recipe(new_recipe);
            cout << "Recipe added successfully!" << endl;
            manager.loadall_Recipes(filename);
            cout << "----------------------------------------\n";
            cout << "Would you like to add this recipe to your favorites? (y/n): ";
            char add_fav;
            cin >> add_fav;
            if (add_fav == 'y' || add_fav == 'Y')
            {
                manager.add_favorite(new_recipe);
                cout << "Recipe added to favorites!" << endl;
            }
        }
            cout << "----------------------------------------\n";
            break;
        default:
            cout << "Invalid choice. Please try again." << endl;
            break;
        }
        cout << "Enter your choice: ";
        cin >> choice;
    }
    cout << "\nThank you for using the Recipe Generator!" << endl;
    cout << "----------------------------------------\n";
    cout << "Happy cooking and enjoy your meal!" << endl;
    return 0;
}