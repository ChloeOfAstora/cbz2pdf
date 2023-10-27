#ifndef MANGATOOLS_GUI_FUNCTIONS_H
#define MANGATOOLS_GUI_FUNCTIONS_H
Fl_Double_Window *mainWindow=(Fl_Double_Window *)0;

Fl_Button *convert_button = nullptr;

Fl_Button *open_button = nullptr;

Fl_Input *resolution_input=(Fl_Input *)0;

Fl_Check_Browser *toggleable_options=nullptr;

Fl_Input *quality_input=(Fl_Input *)0;

Fl_Multi_Browser *input_files = nullptr;

Fl_Button *up_button = nullptr;

Fl_Button *down_button = nullptr;

Fl_Button *remove_button = nullptr;

Fl_Button *about_button = nullptr;

const int width = 300;
const int height = 375;

int selected_index = -1;

struct filePath{
    const char* filePath;
};

void openFilesCallback(Fl_Widget *widget, void *) {
    Fl_File_Chooser file_chooser(".", "*.cbz", Fl_File_Chooser::MULTI, "Select CBZ files");
    file_chooser.show();
    while (file_chooser.shown())
        Fl::wait();
    if (file_chooser.count() > 0) {
        for (int i = 1; i <= file_chooser.count(); i++) {
            std::string filename = file_chooser.value(i);
            std::string fileWithoutPath = filename.substr(filename.find_last_of("/") + 1);
            if (filename.c_str()) {
                filePath* filepath = new filePath;
                filepath->filePath = strdup(filename.c_str());
                input_files->add(fileWithoutPath.c_str(), filepath);
            }
        }
    }
}

void aboutCallback(Fl_Widget *widget, void *) {
    fl_message("This program was made by ChloeOfAstora. \nCopyright 2023\nThird party software:\n\n"
               "| PDFGen -> https://github.com/AndreRenaud/PDFGen\n"
               "| Miniz -> https://github.com/richgel999/miniz\n"
               "| Flags -> https://github.com/sailormoon/flags\n"
               "| stb_image.h -> https://github.com/nothings/stb\n"
               "| stb_image_write.h -> https://github.com/nothings/stb\n"
               "| stb_image_resize.h -> https://github.com/nothings/stb\n"
               "| FLTK -> https://www.fltk.org/index.php\n"
               "\n\n\n\n"
               "miniz LICENSE available at:\n"
               "https://github.com/richgel999/miniz/blob/master/LICENSE"
               );

}

void convertCallback(Fl_Widget *widget, void *data) {
    //Check quality
    std::string quality_value(quality_input->value());
    if(quality_value == "") {
        fl_message("The quality value must be between 0 and 100!");
        return;
    }
    for (char c : quality_value) {
        if(!std::isdigit(c)) {
            fl_message("The quality setting can only contain numbers!");
            return;
        }
    }
    if(std::stoi(quality_input->value()) > 100 || std::stoi(quality_input->value()) < 0) {
        fl_message("The quality value must be between 0 and 100!");
        return;
    }

    int quality = std::stoi(quality_input->value());

    //Check resolution
    std::string resolution_value(resolution_input->value());
    if(resolution_value == "") {
        fl_message("The scale value must be between 0 and 100!");
        return;
    }
    for (char c : resolution_value) {
        if(!std::isdigit(c) && c != '.') {
            fl_message("The scale setting can only contain numbers and periods!");
            return;
        }
    }
    if(std::stof(resolution_input->value()) > 1.0 || std::stof(resolution_input->value()) < 0.0) {
        fl_message("The scale value must be between 0 and 100!");
        return;
    }
    float scale = std::stof(resolution_input->value());

    std::string all_files = "";
    if(input_files->size() < 1) {
        fl_message("You must include at least one file!");
        return;
    }
    for(int i = 1; i <= input_files->size(); ++i) {
        all_files = all_files + " " + static_cast<filePath*>(input_files->data(i))->filePath + " ";
    }

    //std::string full_command = "-r " + std::to_string(scale) + " -q " + std::to_string(quality) + (toggleable_options->checked(1) ? " -g " : "") + (toggleable_options->checked(2) ? " -m " : "") + all_files + " -c";

    for (int i = 1; i <= input_files->size(); ++i) {
        std::string inputFileName = static_cast<filePath*>(input_files->data(i))->filePath;

        // Check file
        std::experimental::filesystem::path inputFilePath(inputFileName);
        if (!std::experimental::filesystem::exists(inputFilePath)) {
            fl_message("Input file does not exist!");
            return;
        }


        // Create a temp folder if it doesn't already exist.
        std::experimental::filesystem::path tempPath(tempName);
        if (!std::experimental::filesystem::exists(tempPath)) {
            std::experimental::filesystem::create_directory(tempName);
        }


        // Extract files from the cbz file to the temp directory
        int num_files = 0;
        if(unzipArchive(inputFileName, num_files) != 0) return;


        // Convert the images and write to the pdf

        if(convertAndWrite(quality, toggleable_options->checked(1), num_files, inputFileName, scale) != 0) return;

        // Clean temp folder
        cleanTemp();
    }
    fl_message("Finished converting!");
}

void selectCallback(Fl_Widget *widget, void *data) {
    Fl_Multi_Browser *browser = (Fl_Multi_Browser *)widget;
    selected_index = browser->value();
    browser->deselect();
    browser->select(selected_index);

}

void upCallback(Fl_Widget *widget, void *data) {
    if(selected_index > 1) {
        input_files->move(selected_index, selected_index - 1);
        selected_index -= 1;
    }
}

void downCallback(Fl_Widget *widget, void *data) {
    if(selected_index < input_files->size()) {
        input_files->move(selected_index, selected_index + 1);
        selected_index += 1;
    }

}

void removeCallback(Fl_Widget *widget, void *data) {
    if(selected_index > 0) {
        input_files->remove(selected_index);
        selected_index = -1;
    }
}
#endif //MANGATOOLS_GUI_FUNCTIONS_H
