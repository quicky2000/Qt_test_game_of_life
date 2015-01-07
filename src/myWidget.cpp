/*
    This file is part of Qt_test_game_of_life
    Copyright (C) 2012  Julien Thevenon ( julien_thevenon at yahoo.fr )

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>
*/
#include <QPainter>
#include <QTimer>
#include <iostream>
#include <map>
using namespace std;

#include "myWidget.h"

myWidget::myWidget(QWidget *parent)
{
  // Geometry initialisation
  m_width = 320;
  m_height = 200;
  m_fill = 0.2;


  m_size = 1280 / m_width ;
  if(1024 / m_height < m_size )
    {
      m_size = 1024 / m_height ;
    }
  this->setGeometry(10, 10, m_size*myWidget::m_width, m_size*myWidget::m_height);
  this->setFixedSize(m_size*myWidget::m_width, m_size*myWidget::m_height);

  // Grid initialisation
  m_grid = new bool*[m_width];
  for(unsigned int i=0;i<m_width;i++)
    {
      m_grid[i] = new bool[m_height] ;
      for(unsigned int j=0;j<m_height;j++)
	{
	  m_grid[i][j]=0;
	}
    }


  // initialisation de générateur à partir de l'heure courante
  srand(time(NULL));
  cout << "RAND_MAX = " << RAND_MAX << endl ;

  unsigned int l_nb = (unsigned int)(m_width * m_height * m_fill);
  for (unsigned int i = 1; i <= l_nb; ++i)
          {
            int l_x = (int)(m_width * ((float)rand())/RAND_MAX);
            int l_y = (int)(m_height * ((float)rand())/RAND_MAX);
           setCellState(l_x,l_y,true);
          }

	  m_timer = new QTimer(this);
  connect(m_timer,SIGNAL(timeout()),this,SLOT(compute()));


};

unsigned int myWidget::getPointId(unsigned int x,unsigned int y)
{
  return m_width * x + y;
}

void myWidget::compute(void)
{
  //  cout << "Compute" << endl ;
  set<unsigned int> l_modified(m_modified);
  m_alive.clear();
  m_modified.clear();
  map<unsigned int,bool> l_new_state;
  set<unsigned int>::iterator l_iter = l_modified.begin();
  set<unsigned int>::iterator l_iter_end = l_modified.end();
  while(l_iter!=l_iter_end)
    {
      unsigned int l_x = getPoint(*l_iter).x();
      unsigned int l_y = getPoint(*l_iter).y();
#ifdef DEBUG
      cout << "Compuite for x = " << l_x << " and y = " << l_y << " qui est " << getCellState(l_x,l_y) << endl ;
#endif

      unsigned int l_nb_voisin = computeNbVoisin(l_x,l_y);
#ifdef DEBUG
      cout << "Voisins = " << l_nb_voisin << endl ;
#endif
      if(getCellState(l_x,l_y))
	{
	  if( l_nb_voisin>3 || l_nb_voisin <2)
	    {
#ifdef DEBUG
	      cout << "Mort" << endl ;
#endif
	      l_new_state[*l_iter]=false;
	    }
	  else
	    {
	      l_new_state[*l_iter]=true;
	    }
	  
	}
      else
	{
	  if( l_nb_voisin>2 && l_nb_voisin <4)
	    {
#ifdef DEBUG
	      cout << "Naissance" << endl ;
#endif
	      l_new_state[*l_iter]=true;
	    }
	}

      l_iter++;
    }

  map<unsigned int,bool>::iterator l_iter2 = l_new_state.begin();
  map<unsigned int,bool>::iterator l_iter2_end = l_new_state.end();
#ifdef DEBUG
  cout << "Nb alive = " << m_alive.size();
#endif
  while(l_iter2 != l_iter2_end)
    {
      unsigned int l_x = getPoint(l_iter2->first).x();
      unsigned int l_y = getPoint(l_iter2->first).y();
      setCellState(l_x,l_y,l_iter2->second);
#ifdef DEBUG
      cout << "setSate x " << l_x << " y = " << l_y << " state = " << l_iter2->second << endl ;
#endif
      l_iter2++;
    }
#ifdef DEBUG
  cout << "Nb alive = " << m_alive.size();
#endif

  update();
}

void myWidget::paintEvent(QPaintEvent * /* event */)
{
  //  cout << "Paint event" << endl ;
  QPainter painter(this);
     painter.setBrush(Qt::blue);
  vector<QPoint>::iterator l_iter = m_alive.begin();
  vector<QPoint>::iterator l_iter_end = m_alive.end();
  while(l_iter != l_iter_end)
      {
        painter.drawRect(l_iter->x()*m_size,l_iter->y()*m_size,m_size,m_size);
	l_iter++;
      }
  
}
  bool myWidget::getCellState(unsigned int x,unsigned int y)
{
  return m_grid[x % m_width][y % m_height];
}

QPoint  myWidget::getPoint(unsigned int p_pointId)
{
  return QPoint(p_pointId/m_width,p_pointId % m_width);
}

unsigned int myWidget::computeNbVoisin(unsigned int l_x,unsigned int l_y)
{
  unsigned int l_result = 0;
  for(int l_difx = -1 ; l_difx < 2 ; l_difx++)
    {
      for(int l_dify = -1 ; l_dify < 2 ; l_dify++)
	{
	  if(l_difx != 0 || l_dify != 0 )
	    { 
	      int l_mx = ( l_x + l_difx + m_width) % m_width ;
	      int l_my = ( l_y + l_dify + m_height) % m_height ;
	      l_result += m_grid[l_mx][l_my];
	    }
	}
    }
  return l_result;
  
}

void myWidget::setCellState(unsigned int x,unsigned int y,bool p_state)
{
  unsigned int l_x = x % m_width;
  unsigned int l_y = y % m_height;
  if(p_state)
    {
      m_alive.push_back(QPoint(l_x,l_y));
    }
  for(int l_difx = -1 ; l_difx < 2 ; l_difx++)
    {
      for(int l_dify = -1 ; l_dify < 2 ; l_dify++)
	{
	  int l_mx = ( l_x + l_difx + m_width ) % m_width ;
	  int l_my = ( l_y + l_dify + m_height ) % m_height ;
	  m_modified.insert(getPointId(l_mx,l_my));
	}
    }
  m_grid[l_x][l_y] = p_state;
}

void myWidget::run(void)
{
  m_timer->start(0);
}

