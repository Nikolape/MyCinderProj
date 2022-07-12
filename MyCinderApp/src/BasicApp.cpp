#include "Circle.h"
#include "Square.h"
#include "Rectangle.h"

#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

using namespace ci;
using namespace ci::app;


class BasicApp : public App {
public:
	BasicApp();
	void mouseDown(MouseEvent event) override;
	void keyDown(KeyEvent event) override;
	void fileDrop(FileDropEvent event) override;
	void draw() override;

	void drawList();
	void drawProperties();
	void drawMenu();
	void saveToFile();
	void loadFromFile();

private:
	std::vector<std::shared_ptr<Shape>> list;
	int selectedObject = -1;
	int selected = 0;
	bool rogue, openMenu;
	std::vector<gl::TextureRef> imgVector;
	gl::FboRef mFbo;
};

BasicApp::BasicApp()
{
	ImGui::Initialize();
	rogue = true;
	openMenu = true;
	randSeed(std::time(NULL));

	mFbo = gl::Fbo::create(640,480);
}

void prepareSettings(BasicApp::Settings* settings)
{
	settings->setMultiTouchEnabled(false);
}

void BasicApp::mouseDown(MouseEvent event) {
	if (event.isLeft())
	{
		switch (randInt(1, 4))
		{
		case 1:
			list.push_back(std::make_shared<Circle>(event.getPos()));
			break;
		case 2:
			list.push_back(std::make_shared<Square>(event.getPos()));
			break;
		case 3:
			list.push_back(std::make_shared<Rectangle>(event.getPos()));
			break;
		default:
			break;
		}
	}
	else
	{
		int remove;
		bool hit = false;
		for (int i = 0; i < list.size(); i++)
		{
			if (list[i]->hit(event.getPos())) {
				remove = i;
				hit = true;
			}
		}
		if (hit) {
			auto it = list.begin();
			it += remove;
			list.erase(it);
		}
	}

}

void BasicApp::keyDown(KeyEvent event)
{
	if (event.getChar() == 'f') {
		setFullScreen(!isFullScreen());
	}
	else if (event.getChar() == 'w') {
		list[list.size() - 1]->moveUp();
	}
	else if (event.getChar() == 's') {
		list[list.size() - 1]->moveDown();
	}
	else if (event.getChar() == 'a') {
		list[list.size() - 1]->moveLeft();
	}
	else if (event.getChar() == 'd') {
		list[list.size() - 1]->moveRight();
	}
	else if (event.getCode() == KeyEvent::KEY_SPACE) {
		list.clear();
	}
	else if (event.getCode() == KeyEvent::KEY_ESCAPE) {
		openMenu = false;
	}
}

void BasicApp::fileDrop(FileDropEvent event)
{
	try {
		imgVector.push_back( gl::Texture::create(loadImage(loadFile(event.getFile(0)))));
	}
	catch (Exception& exc) {
		CI_LOG_EXCEPTION("failed to load image: " << event.getFile(0), exc);
	}
}

void BasicApp::draw()
{
	gl::clear(Color::gray(0.1f));

	mFbo->bindFramebuffer();
	gl::clear();
	gl::color(1.0f, 1.0f, 1.0f);
	//gl::enableAlphaBlending();

	for (int i = 0; i < imgVector.size(); i++)
	{
		
		Rectf destRect(0, (mFbo->getHeight() / imgVector.size()) * i, mFbo->getWidth(), (mFbo->getHeight() / imgVector.size()) * (i + 1));
		Area srcRect(0, (imgVector[i]->getActualHeight() / imgVector.size()) * i, imgVector[i]->getActualWidth(), (imgVector[i]->getActualHeight() / imgVector.size()) * (i + 1));
		gl::draw(imgVector[i], srcRect, destRect);
	}
	
	mFbo->unbindFramebuffer();
	
	gl::draw(mFbo->getColorTexture());

	for (size_t i = 0; i < list.size(); i++)
	{
		list[i]->movment();
	}

	for (size_t i = 0; i < list.size(); i++)
	{
		list[i]->drow();
	}
	
	drawList();
	drawMenu();
	//ImGui::ShowDemoWindow();

}

void BasicApp::drawMenu() {

	ImGui::Begin("Menu");
	if (ImGui::Button("Save")) {
		saveToFile();
	}
	if (ImGui::Button("Load")) {
		loadFromFile();
	}

	ImGui::Checkbox("Rogue", &rogue);
	ImGui::End();
}

void BasicApp::drawList()
{
	ImGui::Begin("list");

	for (int n = 0; n < list.size(); n++)
	{
		char buf[64];
		sprintf_s(buf, "%s %d", list[n]->myShape().c_str(), n + 1);
		if (ImGui::Selectable(buf, selectedObject == n))
		{
			selectedObject = n;
			openMenu = true;
		}

	}
	ImGui::End();


	if (list.empty()) selectedObject = -1;
	if (selectedObject != -1) {
		selected = selectedObject;
		drawProperties();
	}
}

void BasicApp::drawProperties()
{
	if (selected > list.size() - 1) {
		selected = list.size() - 1;
		selectedObject = list.size() - 1;
	}

	if (openMenu) {

		vec2 position = list[selected]->getPosition();
		vec3 color = list[selected]->getColor();
		vec2 force = list[selected]->getForce();
		bool move = list[selected]->getMove();
		if (!ImGui::Begin("Properties", &openMenu))
		{
			ImGui::End();
		}
		else
		{
			ImGui::Text("%s %d", list[selected]->myShape().c_str(), selected + 1);
			if (rogue) {
				ImGui::Checkbox("Move", &move);
				ImGui::SliderFloat("position x", &position.x, 0.0f, app::getWindowWidth());
				ImGui::SliderFloat("position y", &position.y, 0.0f, app::getWindowHeight());
				ImGui::SliderFloat("color R", &color.x, 0.0f, 1.0f);
				ImGui::SliderFloat("color G", &color.y, 0.0f, 1.0f);
				ImGui::SliderFloat("color B", &color.z, 0.0f, 1.0f);
				ImGui::SliderFloat("force x", &force.x, -10.0f, 10.0f);
				ImGui::SliderFloat("force y", &force.y, -10.0f, 10.0f);
			}
			else {
				ImGui::Text("Move: %s", move ? "True" : "False");
				ImGui::Text("Position x: %.3f", position.x);
				ImGui::Text("Position y: %.3f", position.y);
				ImGui::Text("Color R: %.3f", color.x);
				ImGui::Text("Color G: %.3f", color.y);
				ImGui::Text("Color B: %.3f", color.z);
				ImGui::Text("Force x: %.3f", force.x);
				ImGui::Text("Force y: %.3f", force.y);
			}
			ImGui::End();
		}

		if (rogue)
		{
			list[selected]->setPosition(position);
			list[selected]->setColor(color);
			list[selected]->setForce(force);
			list[selected]->setMove(move);
		}
	}
}

void BasicApp::saveToFile()
{
	rapidjson::StringBuffer s;
	rapidjson::Writer<rapidjson::StringBuffer> writer(s);
	writer.StartObject();
		writer.Key("Objects");
			writer.StartArray();
			for (int i = 0; i < list.size(); i++)
			{
				writer.StartObject();
				writer.Key("positionX");
				writer.Double(list[i]->getPosition().x);
				writer.Key("positionY");
				writer.Double(list[i]->getPosition().y);
				writer.Key("colorR");
				writer.Double(list[i]->getColor().x);
				writer.Key("colorG");
				writer.Double(list[i]->getColor().y);
				writer.Key("colorB");
				writer.Double(list[i]->getColor().z);
				writer.Key("forceX");
				writer.Double(list[i]->getForce().x);
				writer.Key("forceY");
				writer.Double(list[i]->getForce().y);
				writer.Key("move");
				writer.Bool(list[i]->getMove());
				writer.Key("myShape");
				writer.Int(list[i]->ID());
				writer.EndObject();
			}
			writer.EndArray();
		writer.Key("Img");
			writer.StartArray();
			for (int i = 0; i < imgVector.size(); i++)
			{
				writer.String("img path");
			}
			writer.EndArray();
	writer.EndObject();
	
	if (std::filesystem::exists("project.csv"))
	{
		if (std::filesystem::exists("project_old.csv")) std::remove("project_old.csv");
		std::rename("project.csv", "project_old.csv");
	}

	std::ofstream file("project.csv");
	file << s.GetString();
	file.close();
}

void BasicApp::loadFromFile()
{
	std::string stringFile;
	std::ifstream file("project.csv");
	std::stringstream strStream;
	strStream << file.rdbuf();
	stringFile = strStream.str();
	file.close();

	vec2 position;
	vec3 color;
	vec2 force;
	bool move;
	int shape;

	rapidjson::Document document;
	document.Parse(stringFile.c_str());
	rapidjson::Value& objects = document["Objects"];
	for (auto it = objects.Begin(); it != objects.End(); ++it)
	{
		position.x = (*it)["positionX"].GetDouble();
		position.y = (*it)["positionY"].GetDouble();
		color.x = (*it)["colorR"].GetDouble();
		color.y = (*it)["colorG"].GetDouble();
		color.z = (*it)["colorB"].GetDouble();
		force.x = (*it)["forceX"].GetDouble();
		force.y = (*it)["forceY"].GetDouble();
		move = (*it)["move"].GetBool();
		shape = (*it)["myShape"].GetInt();
		switch (shape)
		{
		case 1:
			list.push_back(std::make_shared<Circle>(position, color, force, move));
			break;
		case 2:
			list.push_back(std::make_shared<Square>(position, color, force, move));
			break;
		case 3:
			list.push_back(std::make_shared<Rectangle>(position, color, force, move));
			break;
		default:
			break;
		}
	}
}

// This line tells Cinder to actually create and run the application.
CINDER_APP(BasicApp, RendererGl, prepareSettings)
