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
#ifndef MYWIDGET
#define MYWIDGET

#include <QWidget>

#include <vector>
#include <set>
using namespace std;

class myWidget : public QWidget
{
     Q_OBJECT
	  
public:
  myWidget(QWidget *parent = 0);
  void run(void);

 protected:
  void paintEvent(QPaintEvent *event);

  private slots:
  void compute(void);
  
 private:
  bool getCellState(unsigned int x,unsigned int y);
  void setCellState(unsigned int x,unsigned int y,bool p_state);
  unsigned int getPointId(unsigned int x,unsigned int y);
  unsigned int computeNbVoisin(unsigned int x,unsigned int y);
  QPoint getPoint(unsigned int p_pointId);

  set<unsigned int> m_modified;
  vector<QPoint> m_alive;
  unsigned int m_width ;
  unsigned int m_height;
  unsigned int m_size;
  float m_fill;
  QTimer *m_timer;
  bool **m_grid ;
};

#endif
