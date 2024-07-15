/* ----------------------------------- Local -------------------------------- */
#include "project/recent_project_list_delegate.h"

#include "project/recent_project_list_model.h"
/* ------------------------------------ Qt ---------------------------------- */
#include <QFileInfo>
#include <QPainter>
/* -------------------------------------------------------------------------- */

RecentProjectListDelegate::RecentProjectListDelegate(QObject *parent)
    : QStyledItemDelegate(parent),
      m_icon_size(64, 64),
      m_margins(0, 0, 0, 0),
      m_spacing(0, 0) {}

RecentProjectListDelegate::~RecentProjectListDelegate() = default;

void RecentProjectListDelegate::paint(QPainter *painter,
                                      const QStyleOptionViewItem &option,
                                      const QModelIndex &index) const {
  QStyleOptionViewItem opt(option);
  initStyleOption(&opt, index);

  auto &palette = opt.palette;
  auto &rect = opt.rect;
  auto font = QFont(opt.font);
  auto content_rect = rect.adjusted(m_margins.left(), m_margins.top(),
                                    -m_margins.right(), -m_margins.bottom());

  auto is_last_index = index.model()->rowCount() - 1 == index.row();
  auto name =
      index.data(RecentProjectListModel::Role::ProjectNameRole).toString();
  auto path =
      index.data(RecentProjectListModel::Role::ProjectPathRole).toString();
  auto modified_date =
      index.data(RecentProjectListModel::Role::ProjectLastModifiedDateRole)
          .toDateTime()
          .toString();

  painter->save();

  painter->setClipping(true);
  painter->setClipRect(rect);
  painter->setFont(opt.font);

  painter->fillRect(rect, opt.state & QStyle::State_Selected
                              ? palette.highlight().color()
                              : palette.base().color());

  painter->drawLine(is_last_index ? rect.left() : content_rect.left(),
                    rect.bottom(), rect.right(), rect.bottom());

  painter->drawPixmap(content_rect.left(), content_rect.top(),
                      index.data(RecentProjectListModel::Role::ProjectIconRole)
                          .value<QIcon>()
                          .pixmap(m_icon_size));

  auto text_rect =
      content_rect.adjusted(m_icon_size.width() + m_spacing.width(), 0, 0, 0);

  font.setPointSizeF(opt.font.pointSize() * 1.4);
  painter->setFont(font);
  painter->setPen(palette.text().color());
  painter->drawText(text_rect, Qt::TextSingleLine, name);

  auto name_rect = QFontMetrics(font).boundingRect(name).adjusted(0, 0, 1, 1);
  text_rect.moveBottom(text_rect.bottom() + name_rect.height() +
                       m_spacing.height());

  font.setPointSizeF(opt.font.pointSize() * 0.8);
  painter->setFont(font);
  painter->setPen(palette.windowText().color());
  painter->drawText(text_rect, Qt::TextSingleLine, path);

  auto path_rect = QFontMetrics(font).boundingRect(path).adjusted(0, 0, 1, 1);
  text_rect.moveBottom(text_rect.bottom() + path_rect.height());

  font.setPointSizeF(opt.font.pointSize() * 0.6);
  painter->setFont(font);
  painter->setPen(palette.windowText().color());
  painter->drawText(text_rect, Qt::TextSingleLine, modified_date);

  painter->restore();
}

QSize RecentProjectListDelegate::sizeHint(const QStyleOptionViewItem &option,
                                          const QModelIndex &index) const {
  QStyleOptionViewItem opt(option);
  initStyleOption(&opt, index);

  return QSize{opt.rect.width(),
               m_icon_size.height() + m_margins.top() + m_margins.bottom()};
}

void RecentProjectListDelegate::setIconSize(const QSize &size) {
  m_icon_size = size;
}

const QSize &RecentProjectListDelegate::getIconSize() const {
  return m_icon_size;
}

void RecentProjectListDelegate::setMargins(const QMargins &margins) {
  m_margins = margins;
}

const QMargins &RecentProjectListDelegate::getMargins() const {
  return m_margins;
}

void RecentProjectListDelegate::setSpacing(int vertical_spacing,
                                           int horizontal_spacing) {
  m_spacing.setWidth(vertical_spacing);
  m_spacing.setHeight(horizontal_spacing);
}

void RecentProjectListDelegate::setVerticalSpacing(int spacing) {
  m_spacing.setWidth(spacing);
}

void RecentProjectListDelegate::setHorizontalSpacing(int spacing) {
  m_spacing.setHeight(spacing);
}

int RecentProjectListDelegate::getVerticalSpacing() const {
  return m_spacing.width();
}

int RecentProjectListDelegate::getHorizontalSpacing() const {
  return m_spacing.height();
}
