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

import QtQuick 2.0
import Ubuntu.Components 0.1
import "../Components"


DashRenderer {
    id: cardCarousel

    property alias cacheBuffer: carousel.cacheBuffer
    property alias itemComponent: carousel.itemComponent
    property alias minimumTileWidth: carousel.minimumTileWidth
    property alias selectedItemScaleFactor: carousel.selectedItemScaleFactor
    property alias tileAspectRatio: carousel.tileAspectRatio

    currentItem: carousel.currentItem
    height: carousel.implicitHeight + units.gu(6)
    verticalSpacing: units.gu(3)

    Carousel {
        id: carousel
        anchors.fill: parent
        tileAspectRatio: cardCarousel.components && cardCarousel.components["art"]["aspect-ratio"] || 1.0
        // FIXME we need to "reverse" the carousel to make the selected item the size
        // and push others back.
        minimumTileWidth: cardTool.cardWidth / selectedItemScaleFactor
        selectedItemScaleFactor: cardTool.carouselSelectedItemScaleFactor
        cacheBuffer: 1404 // 18px * 13gu * 6
        model: cardCarousel.model

        onClicked: cardCarousel.clicked(index, itemY)
        onPressAndHold: cardCarousel.pressAndHold(index, itemY)

        property real fontScale: 1 / selectedItemScaleFactor
        property real headerHeight: cardTool.headerHeight / selectedItemScaleFactor

        itemComponent: Card {
            id: card
            objectName: "carouselDelegate" + index
            fixedHeaderHeight: carousel.headerHeight
            cardData: model
            template: cardTool.template
            components: cardTool.components

            property bool explicitlyScaled
            property var model

            enabled: false
            showHeader: explicitlyScaled

            fontScale: carousel.fontScale
        }
    }
}