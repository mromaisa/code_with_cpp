#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <windows.h>
using namespace std;

string filename = "recipe.txt";

class Recipe
{
    string recipe_name;
    vector<string> ingredients;
    vector<string> instructions;

public:
    // constructors
    Recipe() : recipe_name(""), ingredients(), instructions() {};
    Recipe(string name, vector<string> ing, vector<string> instr) : recipe_name(name), ingredients(ing), instructions(instr) {};

    // getter functions
    string get_recipe_name() { return recipe_name; }
    vector<string> get_recipe_ingredients() { return ingredients; }
    vector<string> get_recipe_instructions() { return instructions; }

    // display function to display recipe(not menu)
    void display(Recipe r)
    {
        cout << "Recipe Name: " << r.recipe_name << endl;
        cout << "Ingredients: " << endl;
        for (auto i : r.ingredients)
        {
            cout << i << ",";
        }
        cout << endl;
        cout << "Instructions: " << endl;
        for (auto i : r.instructions)
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
                r.instructions.push_back(line.substr(14));
                while ((line.rfind("---", 0) == string::npos) && getline(object, line))
                {
                    r.instructions.push_back(line);
                }
            }
        }
        return r;
    }
};

class Recipe_Manager{
    Recipe r;
    vector<Recipe> recipes;
    public:
    Recipe_Manager(){
        loadall_Recipes();
    }

    void loadall_Recipes(){
        ifstream file(filename);
        if(!file.is_open()){ return; }
        stringstream recipe_textblock;
        string line;
        while(getline(file, line)){
            if(line.rfind("Recipe Name: ", 0) == 0){
                recipe_textblock.str("");
                recipe_textblock << line << endl;
            }
            else if(line.rfind("---", 0) == 0){
                recipes.push_back(r.convert_string_to_recipe(recipe_textblock.str()));
            }
            else{
                recipe_textblock << line << endl;
            }
        }

    }

    void display_menu(){
        for(auto r: recipes){
            r.display(r);
            cout << "------------------------" << endl;
        }
    }

    void search_by_name(string name){
        for(auto r: recipes){
            if(r.get_recipe_name() == name){
                r.display(r);
                return;
            }
        }
        cout << "Recipe not found with that name." << endl;
    }

    void search_by_ingredients(const vector<string> &input_ingredients){
        int max_matches = 0;
        Recipe best_match;
        for(auto &r: recipes){
            int match_count = 0;
            vector<string> recipe_ingredients = r.get_recipe_ingredients();
            for(const auto &ing: input_ingredients){
                for(const auto &recipe_ing: recipe_ingredients){
                    if(ing == recipe_ing){
                        match_count++;
                        break;
                    }
                }
            }
            if(match_count > max_matches){
                max_matches = match_count;
                best_match = r;
            }
        }
        if(max_matches == 0){
            cout << "No matching recipes found." << endl;
        }else{
            best_match.display(best_match);
        }
    }
    
};

int main(){
    cout << "\n-----------------Welcome to a Recipe Generator-------------------\n";
    Recipe_Manager manager;
    int choice;
    cout << "This is a program where you can find recipes(available ingredients / by their name).\n";
    cout << "1. View Menu\n2. Search by Name\n3. Search by Ingredients\n4. Exit\n";
    cout << "--------------------------------------------------------\n";
    cout << "Please select an option from the menu above.\n\n";
    cout << "Enter your choice: ";
    cin >> choice;
    while(choice != 4){
        switch(choice){
            case 1:
                cout << "\n-----------------Menu-------------------\n";
                manager.display_menu();
                break;
            case 2:{
                string name;
                cin.ignore();
                cout << "Enter recipe name: ";
                cout << endl;
                getline(cin, name);
                cout << "----------------------------------------\n";
                manager.search_by_name(name);
                cout << "----------------------------------------\n";
                break;
            }
            case 3:{
                string input;
                cout << "Enter ingredients (comma separated): ";
                cout << endl;
                cin.ignore();
                getline(cin, input);
                cout << "--------------------(Recipes that best matches your ingredients)--------------------\n\n";
                stringstream ss(input);
                string item;
                vector<string> ingredients;
                while(getline(ss, item, ',')){
                    item.erase(0, item.find_first_not_of(" "));
                    ingredients.push_back(item);
                }
                manager.search_by_ingredients(ingredients);
                cout << "------------------------------------------------------------------------------------\n\n";
                break;
            }
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