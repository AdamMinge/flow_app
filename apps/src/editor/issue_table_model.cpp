/* ----------------------------------- Local -------------------------------- */
#include "issue_table_model.h"

#include "resources.h"
/* ---------------------------------- Pillar -------------------------------- */
#include <pillar/issue_manager.h>
/* -------------------------------------------------------------------------- */

IssueTableModel::IssueTableModel(QObject *parent)
    : QAbstractTableModel(parent) {
  auto issue_manager = &pillar::IssueManager::getInstance();

  connect(issue_manager, &pillar::IssueManager::onErrorReport, this,
          &IssueTableModel::addIssue);
  connect(issue_manager, &pillar::IssueManager::onWarningReport, this,
          &IssueTableModel::addIssue);

  connect(issue_manager,
          qOverload<const QVariant &>(&pillar::IssueManager::onClear), this,
          &IssueTableModel::removeIssues);
  connect(issue_manager, qOverload<>(&pillar::IssueManager::onClear), this,
          &IssueTableModel::removeAllIssues);
}

IssueTableModel::~IssueTableModel() = default;

QVariant IssueTableModel::data(const QModelIndex &index, int role) const {
  if (index.row() < 0 || index.row() >= rowCount(index.parent()))
    return QVariant{};

  if (role == Qt::DisplayRole) {
    const auto issue = m_issues[index.row()].first;
    const auto issue_occurrences = m_issues[index.row()].second;
    switch (index.column()) {
      case Column::SeverityColumn:
        return getIssueSeverityName(index);

      case Column::DescriptionColumn:
        return issue.getText();

      case Column::OccurrencesColumn:
        return issue_occurrences;
    }
  } else if (role == Qt::DecorationRole) {
    switch (index.column()) {
      case Column::SeverityColumn:
        return getIssueIcon(index);
    }
  } else if (role == Role::IssueRole) {
    return QVariant::fromValue(m_issues[index.row()].first);
  }

  return QVariant{};
}

int IssueTableModel::rowCount(const QModelIndex &parent) const {
  return static_cast<int>(m_issues.size());
}

int IssueTableModel::columnCount(const QModelIndex &parent) const { return 3; }

QVariant IssueTableModel::headerData(int section, Qt::Orientation orientation,
                                     int role) const {
  if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
    switch (section) {
      case Column::SeverityColumn:
        return tr("Severity");

      case Column::DescriptionColumn:
        return tr("Description");

      case Column::OccurrencesColumn:
        return tr("Occurrences");

      default:
        return QVariant{};
    }
  }
  return QVariant{};
}

void IssueTableModel::addIssue(const pillar::Issue &issue) {
  auto found_issue_iter = std::find_if(
      m_issues.begin(), m_issues.end(),
      [&issue](auto &issue_pair) { return issue_pair.first == issue; });

  if (found_issue_iter == m_issues.end()) {
    beginInsertRows(QModelIndex{}, rowCount(QModelIndex{}),
                    rowCount(QModelIndex{}));
    found_issue_iter =
        m_issues.insert(m_issues.end(), std::make_pair(issue, 1));
    endInsertRows();
  } else {
    found_issue_iter->second += 1;

    auto changed_row =
        static_cast<int>(std::distance(m_issues.begin(), found_issue_iter));
    dataChanged(index(changed_row, Column::OccurrencesColumn),
                index(changed_row, Column::OccurrencesColumn));
  }
}

void IssueTableModel::removeIssues(const QVariant &context) {
  const auto erased_elements = m_issues.removeIf([&context](auto &issue_pair) {
    return issue_pair.first.getContext() == context;
  });

  if (erased_elements > 0) {
    beginResetModel();
    endResetModel();
  }
}

void IssueTableModel::removeAllIssues() {
  if (!m_issues.empty()) {
    beginResetModel();
    m_issues.clear();
    endResetModel();
  }
}

QIcon IssueTableModel::getIssueIcon(const QModelIndex &index) const {
  const auto issue = m_issues[index.row()].first;

  switch (issue.getSeverity()) {
    case pillar::Issue::Severity::Error:
      return QIcon(icons::x32::ErrorIssue);

    case pillar::Issue::Severity::Warning:
      return QIcon(icons::x32::WarningIssue);
  }

  return QIcon{};
}

QString IssueTableModel::getIssueSeverityName(const QModelIndex &index) const {
  const auto issue = m_issues[index.row()].first;

  switch (issue.getSeverity()) {
    case pillar::Issue::Severity::Error:
      return tr("Error");

    case pillar::Issue::Severity::Warning:
      return tr("Warning");
  }

  return QString{};
}
