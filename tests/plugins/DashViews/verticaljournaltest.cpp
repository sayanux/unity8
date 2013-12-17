/*
 * Copyright (C) 2013 Canonical, Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 3.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <QQuickView>
#include <QtTestGui>
#include <QDebug>
#include <QGuiApplication>
#include <QQuickView>
#include <QtQml/qqml.h>
#include <QStringListModel>
#include <QQmlContext>
#include <QQmlEngine>
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-pedantic"
#include <private/qquickitem_p.h>
#pragma GCC diagnostic pop

#include "verticaljournal.h"

class QHeightModel : public QStringListModel {
public:
    QHash<int, QByteArray> roleNames() const
    {
        QHash<int, QByteArray> roles;
        roles.insert(Qt::DisplayRole, "modelHeight");
        return roles;
    }
};

class VerticalJournalTest : public QObject
{
    Q_OBJECT

private:
    void verifyItem(const VerticalJournal::ViewItem &item, int modelIndex, qreal x, qreal y, bool visible)
    {
        QTRY_COMPARE(item.m_modelIndex, modelIndex);
        QTRY_COMPARE(item.x(), x);
        QTRY_COMPARE(item.y(), y);
        QTRY_COMPARE(item.height(), heightList[modelIndex].toDouble());
        QTRY_COMPARE(QQuickItemPrivate::get(item.m_item)->culled, !visible);
    }

    void checkInitialPositions()
    {
        QTRY_COMPARE(vj->m_columnVisibleItems.count(), 3);
        QTRY_COMPARE(vj->m_columnVisibleItems[0].count(), 5);
        QTRY_COMPARE(vj->m_columnVisibleItems[1].count(), 7);
        QTRY_COMPARE(vj->m_columnVisibleItems[2].count(), 6);
        verifyItem(vj->m_columnVisibleItems[0][0],  0,   0,   0, true);
        verifyItem(vj->m_columnVisibleItems[1][0],  1, 160,   0, true);
        verifyItem(vj->m_columnVisibleItems[2][0],  2, 320,   0, true);
        verifyItem(vj->m_columnVisibleItems[1][1],  3, 160,  60, true);
        verifyItem(vj->m_columnVisibleItems[1][2],  4, 160,  80, true);
        verifyItem(vj->m_columnVisibleItems[0][1],  5,   0, 110, true);
        verifyItem(vj->m_columnVisibleItems[1][3],  6, 160, 130, true);
        verifyItem(vj->m_columnVisibleItems[2][1],  7, 320, 135, true);
        verifyItem(vj->m_columnVisibleItems[0][2],  8,   0, 190, true);
        verifyItem(vj->m_columnVisibleItems[2][2],  9, 320, 255, true);
        verifyItem(vj->m_columnVisibleItems[2][3], 10, 320, 285, true);
        verifyItem(vj->m_columnVisibleItems[2][4], 11, 320, 315, true);
        verifyItem(vj->m_columnVisibleItems[1][4], 12, 160, 340, true);
        verifyItem(vj->m_columnVisibleItems[0][3], 13,   0, 360, true);
        verifyItem(vj->m_columnVisibleItems[2][5], 14, 320, 390, true);
        verifyItem(vj->m_columnVisibleItems[1][5], 15, 160, 430, false);
        verifyItem(vj->m_columnVisibleItems[0][4], 16,   0, 570, false);
        verifyItem(vj->m_columnVisibleItems[1][6], 17, 160, 570, false);
        QCOMPARE(vj->implicitHeight(), 970. + 2. * 970. / 18.);
    }

private Q_SLOTS:
    void init()
    {
        view = new QQuickView();
        view->setResizeMode(QQuickView::SizeRootObjectToView);
        view->engine()->addImportPath(BUILT_PLUGINS_DIR);

        model = new QHeightModel();
        heightList.clear();
        heightList << "100" << "50" << "125" << "10" << "40" << "70" << "200" << "110" << "160" << "20" << "20" << "65" << "80" << "200" << "300" << "130" << "400" << "300" << "500" << "10";
        model->setStringList(heightList);

        view->setSource(QUrl::fromLocalFile(DASHVIEWSTEST_FOLDER "/verticaljournaltest.qml"));

        view->show();
        view->resize(470, 400);
        QTest::qWaitForWindowExposed(view);

        vj = dynamic_cast<VerticalJournal*>(view->rootObject()->findChild<QObject*>("vj"));
        vj->setModel(model);

        checkInitialPositions();
    }

    void cleanup()
    {
        delete view;
        delete model;
    }

    void testWidthResize()
    {
        view->resize(629, 400);
        QTRY_COMPARE(vj->width(), 629.);

        // This is exactly the same block as above as nothing changed, just white space on the right
        checkInitialPositions();

        view->resize(630, 400);

        QTRY_COMPARE(vj->m_columnVisibleItems.count(), 4);
        QTRY_COMPARE(vj->m_columnVisibleItems[0].count(), 4);
        QTRY_COMPARE(vj->m_columnVisibleItems[1].count(), 7);
        QTRY_COMPARE(vj->m_columnVisibleItems[2].count(), 4);
        QTRY_COMPARE(vj->m_columnVisibleItems[3].count(), 5);
        verifyItem(vj->m_columnVisibleItems[0][0],  0,   0,   0, true);
        verifyItem(vj->m_columnVisibleItems[1][0],  1, 160,   0, true);
        verifyItem(vj->m_columnVisibleItems[2][0],  2, 320,   0, true);
        verifyItem(vj->m_columnVisibleItems[3][0],  3, 480,   0, true);
        verifyItem(vj->m_columnVisibleItems[3][1],  4, 480,  20, true);
        verifyItem(vj->m_columnVisibleItems[1][1],  5, 160,  60, true);
        verifyItem(vj->m_columnVisibleItems[3][2],  6, 480,  70, true);
        verifyItem(vj->m_columnVisibleItems[0][1],  7,   0, 110, true);
        verifyItem(vj->m_columnVisibleItems[2][1],  8, 320, 135, true);
        verifyItem(vj->m_columnVisibleItems[1][2],  9, 160, 140, true);
        verifyItem(vj->m_columnVisibleItems[1][3], 10, 160, 170, true);
        verifyItem(vj->m_columnVisibleItems[1][4], 11, 160, 200, true);
        verifyItem(vj->m_columnVisibleItems[0][2], 12,   0, 230, true);
        verifyItem(vj->m_columnVisibleItems[1][5], 13, 160, 275, true);
        verifyItem(vj->m_columnVisibleItems[3][3], 14, 480, 280, true);
        verifyItem(vj->m_columnVisibleItems[2][2], 15, 320, 305, true);
        verifyItem(vj->m_columnVisibleItems[0][3], 16,   0, 320, true);
        verifyItem(vj->m_columnVisibleItems[2][3], 17, 320, 445, false);
        verifyItem(vj->m_columnVisibleItems[1][6], 18, 160, 485, false);
        verifyItem(vj->m_columnVisibleItems[3][4], 19, 480, 590, false);
        QTRY_COMPARE(vj->implicitHeight(), 985.);

        view->resize(470, 400);

        // This is exactly the same block as the first again
        checkInitialPositions();
    }

    void testHorizontalSpacing()
    {
        vj->setColumnSpacing(11);

        QTRY_COMPARE(vj->m_needsRelayout, false);
        QTRY_COMPARE(vj->m_columnVisibleItems.count(), 2);
        QTRY_COMPARE(vj->m_columnVisibleItems[0].count(), 7);
        QTRY_COMPARE(vj->m_columnVisibleItems[1].count(), 7);
        verifyItem(vj->m_columnVisibleItems[0][0],  0,   0,   0, true);
        verifyItem(vj->m_columnVisibleItems[1][0],  1, 161,   0, true);
        verifyItem(vj->m_columnVisibleItems[1][1],  2, 161,  60, true);
        verifyItem(vj->m_columnVisibleItems[0][1],  3,   0, 110, true);
        verifyItem(vj->m_columnVisibleItems[0][2],  4,   0, 130, true);
        verifyItem(vj->m_columnVisibleItems[0][3],  5,   0, 180, true);
        verifyItem(vj->m_columnVisibleItems[1][2],  6, 161, 195, true);
        verifyItem(vj->m_columnVisibleItems[0][4],  7,   0, 260, true);
        verifyItem(vj->m_columnVisibleItems[0][5],  8,   0, 380, true);
        verifyItem(vj->m_columnVisibleItems[1][3],  9, 161, 405, false);
        verifyItem(vj->m_columnVisibleItems[1][4], 10, 161, 435, false);
        verifyItem(vj->m_columnVisibleItems[1][5], 11, 161, 465, false);
        verifyItem(vj->m_columnVisibleItems[1][6], 12, 161, 540, false);
        verifyItem(vj->m_columnVisibleItems[0][6], 13,   0, 550, false);
        QCOMPARE(vj->implicitHeight(), 750. + 6. * 750. / 14.);

        vj->setColumnSpacing(10);
        QTRY_COMPARE(vj->m_needsRelayout, false);

        // This is exactly the same block as the first again
        checkInitialPositions();
    }

    void testVerticalSpacing()
    {
        vj->setRowSpacing(11);

        QTRY_COMPARE(vj->m_needsRelayout, false);
        QTRY_COMPARE(vj->m_columnVisibleItems.count(), 3);
        QTRY_COMPARE(vj->m_columnVisibleItems[0].count(), 5);
        QTRY_COMPARE(vj->m_columnVisibleItems[1].count(), 7);
        QTRY_COMPARE(vj->m_columnVisibleItems[2].count(), 6);
        verifyItem(vj->m_columnVisibleItems[0][0],  0,   0,   0, true);
        verifyItem(vj->m_columnVisibleItems[1][0],  1, 160,   0, true);
        verifyItem(vj->m_columnVisibleItems[2][0],  2, 320,   0, true);
        verifyItem(vj->m_columnVisibleItems[1][1],  3, 160,  61, true);
        verifyItem(vj->m_columnVisibleItems[1][2],  4, 160,  82, true);
        verifyItem(vj->m_columnVisibleItems[0][1],  5,   0, 111, true);
        verifyItem(vj->m_columnVisibleItems[1][3],  6, 160, 133, true);
        verifyItem(vj->m_columnVisibleItems[2][1],  7, 320, 136, true);
        verifyItem(vj->m_columnVisibleItems[0][2],  8,   0, 192, true);
        verifyItem(vj->m_columnVisibleItems[2][2],  9, 320, 257, true);
        verifyItem(vj->m_columnVisibleItems[2][3], 10, 320, 288, true);
        verifyItem(vj->m_columnVisibleItems[2][4], 11, 320, 319, true);
        verifyItem(vj->m_columnVisibleItems[1][4], 12, 160, 344, true);
        verifyItem(vj->m_columnVisibleItems[0][3], 13,   0, 363, true);
        verifyItem(vj->m_columnVisibleItems[2][5], 14, 320, 395, true);
        verifyItem(vj->m_columnVisibleItems[1][5], 15, 160, 435, false);
        verifyItem(vj->m_columnVisibleItems[0][4], 16,   0, 574, false);
        verifyItem(vj->m_columnVisibleItems[1][6], 17, 160, 576, false);
        QCOMPARE(vj->implicitHeight(), 974. + 2. * 974. / 18.);

        vj->setRowSpacing(10);
        QTRY_COMPARE(vj->m_needsRelayout, false);

        // This is exactly the same block as the first again
        checkInitialPositions();
    }

    void testDelegateCreationRanges()
    {
        vj->setDelegateCreationBegin(200);
        vj->setDelegateCreationEnd(view->height());

        QTRY_COMPARE(vj->m_columnVisibleItems.count(), 3);
        QTRY_COMPARE(vj->m_columnVisibleItems[0].count(), 4);
        QTRY_COMPARE(vj->m_columnVisibleItems[1].count(), 4);
        QTRY_COMPARE(vj->m_columnVisibleItems[2].count(), 6);
        verifyItem(vj->m_columnVisibleItems[0][0],  0,   0,   0, false);
        verifyItem(vj->m_columnVisibleItems[2][0],  2, 320,   0, false);
        verifyItem(vj->m_columnVisibleItems[1][0],  4, 160,  80, false);
        verifyItem(vj->m_columnVisibleItems[0][1],  5,   0, 110, false);
        verifyItem(vj->m_columnVisibleItems[1][1],  6, 160, 130, true);
        verifyItem(vj->m_columnVisibleItems[2][1],  7, 320, 135, true);
        verifyItem(vj->m_columnVisibleItems[0][2],  8,   0, 190, true);
        verifyItem(vj->m_columnVisibleItems[2][2],  9, 320, 255, true);
        verifyItem(vj->m_columnVisibleItems[2][3], 10, 320, 285, true);
        verifyItem(vj->m_columnVisibleItems[2][4], 11, 320, 315, true);
        verifyItem(vj->m_columnVisibleItems[1][2], 12, 160, 340, true);
        verifyItem(vj->m_columnVisibleItems[0][3], 13,  0,  360, true);
        verifyItem(vj->m_columnVisibleItems[2][5], 14, 320, 390, true);
        verifyItem(vj->m_columnVisibleItems[1][3], 15, 160, 430, false);
        QCOMPARE(vj->implicitHeight(), 690. + 4. * 690. / 16.);

        vj->resetDelegateCreationBegin();
        vj->resetDelegateCreationEnd();

        // This is exactly the same block as the first again
        checkInitialPositions();
    }


    void testColumnWidthChange()
    {
        vj->setColumnWidth(200);
        QTRY_COMPARE(vj->m_columnVisibleItems.count(), 2);
        QTRY_COMPARE(vj->m_columnVisibleItems[0].count(), 7);
        QTRY_COMPARE(vj->m_columnVisibleItems[1].count(), 7);
        verifyItem(vj->m_columnVisibleItems[0][0],  0,   0,   0, true);
        verifyItem(vj->m_columnVisibleItems[1][0],  1, 210,   0, true);
        verifyItem(vj->m_columnVisibleItems[1][1],  2, 210,  60, true);
        verifyItem(vj->m_columnVisibleItems[0][1],  3,   0, 110, true);
        verifyItem(vj->m_columnVisibleItems[0][2],  4,   0, 130, true);
        verifyItem(vj->m_columnVisibleItems[0][3],  5,   0, 180, true);
        verifyItem(vj->m_columnVisibleItems[1][2],  6, 210, 195, true);
        verifyItem(vj->m_columnVisibleItems[0][4],  7,   0, 260, true);
        verifyItem(vj->m_columnVisibleItems[0][5],  8,   0, 380, true);
        verifyItem(vj->m_columnVisibleItems[1][3],  9, 210, 405, false);
        verifyItem(vj->m_columnVisibleItems[1][4], 10, 210, 435, false);
        verifyItem(vj->m_columnVisibleItems[1][5], 11, 210, 465, false);
        verifyItem(vj->m_columnVisibleItems[1][6], 12, 210, 540, false);
        verifyItem(vj->m_columnVisibleItems[0][6], 13,   0, 550, false);
        QCOMPARE(vj->implicitHeight(), 750. + 6. * 750. / 14.);

        vj->setColumnWidth(150);

        // This is exactly the same block as the first again
        checkInitialPositions();
    }

    void testChangeModel()
    {
        QHeightModel model2;
        QStringList list2;
        list2 << "100" << "50" << "25" << "25" << "50" << "50";
        model2.setStringList(list2);
        vj->setModel(&model2);
        heightList = list2;

        QTRY_COMPARE(vj->m_columnVisibleItems.count(), 3);
        QTRY_COMPARE(vj->m_columnVisibleItems[0].count(), 1);
        QTRY_COMPARE(vj->m_columnVisibleItems[1].count(), 2);
        QTRY_COMPARE(vj->m_columnVisibleItems[2].count(), 3);
        verifyItem(vj->m_columnVisibleItems[0][0],  0,   0,  0, true);
        verifyItem(vj->m_columnVisibleItems[1][0],  1, 160,  0, true);
        verifyItem(vj->m_columnVisibleItems[2][0],  2, 320,  0, true);
        verifyItem(vj->m_columnVisibleItems[2][1],  3, 320, 35, true);
        verifyItem(vj->m_columnVisibleItems[1][1],  4, 160, 60, true);
        verifyItem(vj->m_columnVisibleItems[2][2],  5, 320, 70, true);
    }

private:
    QQuickView *view;
    VerticalJournal *vj;
    QStringList heightList;
    QHeightModel *model;
};

QTEST_MAIN(VerticalJournalTest)

#include "verticaljournaltest.moc"
