
#include "SourceStates/game_state.h"
#include "SourceModule/modules.h"
#include "SourceOverlays/Overlays.h"
#include "SourceStations/stations.h"
#include "SourceCore/colide.h"

void SubMain::inhand_logic()
{
    if(inHand.size() != 0)
    {
        if(gameMain->keyStates[SDL_SCANCODE_Q])
        {
            inHandTechRot += windowRotSpeed;
        }
        if(gameMain->keyStates[SDL_SCANCODE_E])
        {
            inHandTechRot -= windowRotSpeed;
        }
        if(gameMain->keyStates[SDL_SCANCODE_Z])
        {
            inHandTechRot -= inHandRotSpeed;
        }
        if(gameMain->keyStates[SDL_SCANCODE_X])
        {
            inHandTechRot += inHandRotSpeed;
        }
        //fix inHandtech
        if(inHandTechRot < 0)
        {
            inHandTechRot += 360;
        }
        if(inHandTechRot > 360)
        {
            inHandTechRot -= 360;
        }
        rotate_inHand(inHandTechRot - inHand[0]->tex->texCoords->rot);
        handle_locking();
        handle_no_place();
    }

}

/*void SubMain::handle_pickup()
{
    for(int i = 0;i < station->modules.size();i++)//put modules inHand
    {
        if(lp_point(gameMousePos,*station->modules[i]->hit))
        {
            inHandTechRot = station->modules[i]->tex->texCoords->rot;
            if(station->modules[i]->selectedi->stored)
            {
                add_inHand(station->modules[i]);
                for(int j = 0;j < selected.size();j++)//addin all the selected mods
                {
                    if(station->modules[i] != selected[j])//make sure it was not already placed
                    {
                        add_inHand(selected[j]);
                    }
                }
            }
            else//clicked on one was not selected
            {
                add_inHand(station->modules[i]);
            }
            break;
        }
    }
    if(inHand.size() > 0)//if any modules were put inHand
    {
        for(int h = 0;h < inHand.size();h++)//scroll through all the things in Hand
        {
            for(int i = 0;i < inHand[h]->nodes.size();i++)//scroll thorugh InHand nodes
            {
                if(inHand[h]->nodes[i]->adjacent != NULL && !inHand[h]->nodes[i]->adjacent->inHandi->stored)//the node is connected to something that is not inHand
                {
                    inHand[h]->nodes[i]->open();
                }
            }
        }
    }
}*/

void SubMain::handle_locking()
{
    if(overlays[1]->get_val(0))
    {
        //this section is the original cliping algorithm. It used to work better
        /*if(inHand.size() == 1)//keep this because it has better stuff when the mouse is directly over another module
        {
            std::vector<int> closest;//is location in vector
            for(int i = 0;i < station->modules.size();i++)//find closest module
            {
                float dist = std::sqrt(std::pow(inHand[0]->tex->texCoords->orig.x - station->modules[i]->tex->texCoords->orig.x,2) +
                                       std::pow(inHand[0]->tex->texCoords->orig.y - station->modules[i]->tex->texCoords->orig.y,2));
                if(dist < station->modules[i]->hit->find_radius() + inHand[0]->hit->find_radius() && dist > 1)//so it is not itself
                {
                    closest.push_back(i);
                }
            }
            if(closest.size() > 0)//if there is one closer than 500 units
            {
                std::vector<Point> best;//x is node in Hand,y is node in module
                std::vector<Point> aux;//x is dist,y is module
                std::vector<Point>::iterator it;
                best.push_back(Point(-1,-1));
                aux.push_back(Point(2000,-1));
                for(int i = 0;i < closest.size();i++)//scroll through possibilities
                {
                    for(int j = 0;j < inHand[0]->nodes.size();j++)//through inHand nodes
                    {
                        for(int k = 0;k < station->modules[closest[i]]->nodes.size();k++)//and the possibilities nodes
                        {
                            if(station->modules[closest[i]]->nodes[k]->adjacent == NULL)//possible node is open (inHand is open for sure)
                            {
                                float tempDist = inHand[0]->nodes[j]->get_dif(station->modules[closest[i]]->nodes[k]).get_dist();
                                for(int h = 0;h < aux.size();h++)//aux should be the same size as best
                                {
                                    if(tempDist < aux[h].x)
                                    {
                                        it = aux.begin() + h;
                                        aux.insert(it,Point(tempDist,closest[i]));
                                        it = best.begin() + h;
                                        best.insert(it,Point(j,k));
                                        break;
                                    }
                                }
                            }
                        }
                    }
                }
                Point bestDist(-1,2000);// x is loc in aux //y is distance
                for(int i = 0;i < aux.size() - 1;i++)//-1 so it doesn't reach (2000,-1) place holder
                {
                    inHand[0]->translate_m(gameMousePos.x,gameMousePos.y,true);
                    inHand[0]->rotate(station->modules[aux[i].y]->tex->texCoords->rot - inHand[0]->tex->texCoords->rot);//match orientation //further equations assume that this is true
                    inHand[0]->rotate(station->modules[aux[i].y]->nodes[best[i].y]->rotation - inHand[0]->nodes[best[i].x]->rotation + 180);
                    Point temp = (station->modules[aux[i].y]->nodes[best[i].y]->tex->texCoords->orig +
                                (inHand[0]->nodes[best[i].x]->tex->texCoords->orig * -1)) +
                                station->modules[aux[i].y]->tex->texCoords->orig;
                    inHand[0]->translate_m(temp.x,temp.y,true);
                    if(std::sqrt(std::pow(gameMousePos.x - temp.x,2) +
                                 std::pow(gameMousePos.y - temp.y,2)) < bestDist.y &&
                       !lp_rect(*inHand[0]->hit,*station->modules[aux[i].y]->hit))//if they are not coliding
                    {
                        bestDist.x = i;
                        bestDist.y = std::sqrt(std::pow(gameMousePos.x - temp.x,2) +
                                               std::pow(gameMousePos.y - temp.y,2));
                    }
                }
                inHand[0]->translate_m(gameMousePos.x,gameMousePos.y,true);
                inHand[0]->rotate(station->modules[aux[bestDist.x].y]->tex->texCoords->rot - inHand[0]->tex->texCoords->rot);//match orientation //further equations assume that this is true
                inHand[0]->rotate(station->modules[aux[bestDist.x].y]->nodes[best[bestDist.x].y]->rotation - inHand[0]->nodes[best[bestDist.x].x]->rotation + 180);
                Point temp = (station->modules[aux[bestDist.x].y]->nodes[best[bestDist.x].y]->tex->texCoords->orig +
                                (inHand[0]->nodes[best[bestDist.x].x]->tex->texCoords->orig * -1)) +
                                station->modules[aux[bestDist.x].y]->tex->texCoords->orig;
                inHand[0]->translate_m(temp.x,temp.y,true);
            }
        }
        else */
        if(inHand.size() > 0)
        {
            std::vector<Node *> inHandNodes;//the open nnodes inHand
            std::vector<Node *> optionNodes;//open options
            for(int i = 0;i < station->modules.size();i++)//go through all modules
            {
                for(int j = 0;j < station->modules[i]->nodes.size();j++)
                {
                    if(station->modules[i]->nodes[j]->adjacent == NULL)//it is open
                    {
                        if(station->modules[i]->inHandi->stored)
                        {
                            inHandNodes.push_back(station->modules[i]->nodes[j]);
                        }
                        else
                        {
                            optionNodes.push_back(station->modules[i]->nodes[j]);
                        }
                    }
                }
            }
            int bestInHand = -1;
            int bestOption = -1;
            float bestDist = 2000;
            for(int i = 0;i < inHandNodes.size();i++)
            {
                for(int j = 0;j < optionNodes.size();j++)//scroll through option nodes and inhands
                {
                    Point temp = inHandNodes[i]->get_dif(optionNodes[j]);
                    if(std::abs(temp.x) < bestDist &&
                       std::abs(temp.y) < bestDist)//if they are close enough for it to be worht pythagorean
                    {
                        float tempdist = temp.get_dist();
                        if(tempdist < bestDist)
                        {
                            bestInHand = i;
                            bestOption = j;
                            bestDist = tempdist;
                        }
                    }
                }
            }
            if(bestDist < 50)
            {
                float r = optionNodes[bestOption]->parent->tex->texCoords->rot - inHandNodes[bestInHand]->parent->tex->texCoords->rot;
                r += optionNodes[bestOption]->rotation - inHandNodes[bestInHand]->rotation + 180;
                rotate_inHand(r);
                Point dif = optionNodes[bestOption]->get_dif(inHandNodes[bestInHand]);
                for(int i = 0;i < inHand.size();i++)
                {
                    inHand[i]->translate_m(dif.x,dif.y,false);
                }
            }
        }
    }
}

void SubMain::handle_placement()
{
    if(inHand.size() > 0 && !noPlaceBool)
    {
        if(overlays[OverlayType::EDITOR]->get_val(0))
        {
            //make options and inhand list
            std::vector<Node *> inHandNodes;//the open nnodes inHand
            std::vector<Node *> optionNodes;//open options
            for(int i = 0;i < station->modules.size();i++)//go through all modules
            {
                for(int j = 0;j < station->modules[i]->nodes.size();j++)
                {
                    if(station->modules[i]->nodes[j]->adjacent == NULL)//it is open
                    {
                        if(station->modules[i]->inHandi->stored)
                        {
                            inHandNodes.push_back(station->modules[i]->nodes[j]);
                        }
                        else
                        {
                            optionNodes.push_back(station->modules[i]->nodes[j]);
                        }
                    }
                }
            }
            for(int i = 0;i < inHandNodes.size();i++)
            {
                for(int j = 0;j < optionNodes.size();j++)
                {
                    Point temp = inHandNodes[i]->get_dif(optionNodes[j]);
                    if(std::abs(temp.x) < 1  &&
                       std::abs(temp.y) < 1)//if the nodes are allined
                    {
                        inHandNodes[i]->connect(optionNodes[j]);
                    }
                }
            }
        }
        for(int i = 0;i < inHand.size();i++)
        {
            inHand[i]->inHandi->stored = false;
        }
        inHand.clear();
        inHandOffsets.clear();
    }
}

void SubMain::add_inHand(Module * m)
{
    inHand.push_back(m);
    inHand[inHand.size() - 1]->inHandi->stored = true;
    inHand[inHand.size() - 1]->inHandi->index = inHand.size() - 1;
    inHandOffsets.push_back(Point(m->tex->texCoords->orig.x - gameMousePos.x,
                                  m->tex->texCoords->orig.y - gameMousePos.y));
}

void SubMain::rotate_inHand(float r)
{
    for(int i = 0;i < inHand.size();i++)
    {
        inHandOffsets[i].rotate(r);
        inHand[i]->rotate(r);
        inHand[i]->translate_m(gameMousePos.x + inHandOffsets[i].x,
                               gameMousePos.y + inHandOffsets[i].y,true);
    }
}

void SubMain::delete_inHand()
{
	for (int i = 0; i < inHand.size(); i++)//scroll through inHand
	{
		for (int j = 0; j < inHand[i]->nodes.size(); j++)//an its nodes
		{
			if (inHand[i]->nodes[j]->adjacent != NULL)//it is connected to something
			{
				inHand[i]->nodes[j]->open();
			}
		}
		overlays[OverlayType::EDITOR]->change(inHand[i]->type);
    }
    for(int i = 0;i < station->modules.size();i++)//scroll through modules // it is accountedd for that modules.size() will change
    {
        if(station->modules[i]->inHandi->stored)//the modules is inHand
        {
            station->modules.erase(station->modules.begin() + i);
            i--;//so that it stays in this spot and doesn't skip the next one
        }
    }
    inHand.clear();
}

void SubMain::destroy_selection()
{
	create_popups(num_to_string(selected.size()) + " Moudles Destroyed for " + num_to_string(selected.size() * 5) + "$");
	gameMain->money -= selected.size() * 5;
	for (int i = 0; i < selected.size(); i++)
	{
		for (int j = 0; j < selected[i]->nodes.size(); j++)//and its nodes
		{
			if (selected[i]->nodes[j]->adjacent != NULL)//it is connected to something
			{
				selected[i]->nodes[j]->open();
			}
		}
	}
	for (int i = 0; i < station->modules.size(); i++)//scroll through modules // it is accountedd for that modules.size() will change
	{
		if (station->modules[i]->selectedi->stored)//the modules is inHand
		{
			station->modules.erase(station->modules.begin() + i);
			i--;//so that it stays in this spot and doesn't skip the next one
		}
	}
	for (int i = 0; i < selected.size(); i++) {
		delete selected[i];
	}
	selected.clear();
}

void SubMain::handle_no_place()
{
    noPlaceBool = false;
    for(int i = 0;i < station->modules.size() && !noPlaceBool;i++)
    {
        if(!station->modules[i]->inHandi->stored)//modules[i] is not inHand
        {
            for(int j = 0;j < inHand.size();j++)
            {
                if(lp_rect(*station->modules[i]->hit,*inHand[j]->hit))
                {
                    noPlaceBool = true;
                    break;
                }
            }
        }
    }
    if(station->stage_type == 1)
    {
        for(int i = 0;i < inHand.size();i++)
        {
            if(inHand[i]->hit->orig.get_dist() > 3500)
            {
                noPlaceBool = true;
                break;
            }
        }
    }
}

void SubMain::render_no_place()
{
    for(int i = 0;i < inHand.size();i++)
    {
        inHand[i]->hit->render(1,0,0,.5);
    }
}

