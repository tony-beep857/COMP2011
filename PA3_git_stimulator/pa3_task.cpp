#include <cstring>

#include "constants.h"
#include "given.h"
#include "pa3_task.h"
#include "structures.h"

using namespace std;

// ─────────────────────────────────────────────────────────────
// Task 1 - Register New User
// ─────────────────────────────────────────────────────────────
const User *register_new_user(UserManagement &userManagement,
                              const char *username)
{
  // 1. Validation: Check if the username length exceeds the limit
  // MAX_USER_NAME_LEN includes the null terminator, so we check >=
  if (strlen(username) >= MAX_USER_NAME_LEN)
  {
    cout << "Username length exceeded " << MAX_USER_NAME_LEN - 1 << " characters." << endl;
    return nullptr;
  }

  // 2. Traversal: Find the insertion point and check for duplicates
  User *prev = nullptr;
  User *curr = userManagement.head;

  while (curr != nullptr)
  {
    int cmp = strcmp(curr->name, username);

    if (cmp == 0)
    {
      // Validation: User already exists
      cout << "User " << username << " already exists." << endl;
      return nullptr;
    }

    if (cmp > 0)
    {
      // Found the first node that is alphabetically "greater" than our new user
      // This is where we stop to insert before 'curr'
      break;
    }

    // Move to the next node
    prev = curr;
    curr = curr->next;
  }

  // 3. Creation: Allocate and initialize the new User
  User *newUser = new User;
  strcpy(newUser->name, username);
  newUser->numRepos = 0;
  newUser->repos = nullptr;

  // 4. Insertion: Update pointers to maintain the sorted linked list
  newUser->next = curr;

  if (prev == nullptr)
  {
    // The list was empty OR the new user is the smallest alphabetically
    userManagement.head = newUser;
  }
  else
  {
    // Insert between prev and curr
    prev->next = newUser;
  }

  return newUser;
}

// ─────────────────────────────────────────────────────────────
// Task 2 - Create Repository
// Main branch is represented implicitly by repo->commits.
// It is NEVER stored in branches[] and numBranches stays 0.
// ─────────────────────────────────────────────────────────────
int create_repository(RepositoryManagement &repoManagement, User *owner,
                      const char *repoName, const time_t creationTimestamp)
{
  if (strlen(repoName) > MAX_REPO_NAME_LEN)
  {
    cout << "Repository name length exceeded " << MAX_REPO_NAME_LEN - 1
         << " characters." << endl;
    return -1;
  }
  for (int i = 0; i < repoManagement.numRepos; i++)
  {
    Repository *r = repoManagement.repos[i];
    if (r->owner == owner && strcmp(r->name, repoName) == 0)
    {
      cout << "Repository " << owner->name << '/' << repoName
           << " already exists." << endl;
      return -1;
    }
  }

  // Initial commit
  Commit *initialCommit = new Commit;
  initialCommit->author = owner;
  initialCommit->timestamp = creationTimestamp;
  initialCommit->next = nullptr;
  initialCommit->prev = nullptr;
  strcpy(initialCommit->message, "Initial commit.");

  SHA1 sha;
  initialize(sha);
  input(sha, owner->name, strlen(owner->name));
  input(sha, initialCommit->message, strlen(initialCommit->message));
  input(sha, creationTimestamp);
  digest(sha);
  initialCommit->hash = sha;

  // Repository
  Repository *newRepo = new Repository;
  newRepo->owner = owner;
  newRepo->numPrs = 0;
  newRepo->numForks = 0;
  newRepo->numBranches = 0;
  newRepo->prs = nullptr;
  newRepo->forks = nullptr;
  newRepo->branches = nullptr;
  newRepo->commits = initialCommit; // main head
  strcpy(newRepo->name, repoName);

  // Add to global repo array
  // 3. 维护全局 RepositoryManagement 数组 (双重字典序)
  // 先按 owner->name 排序，再按 repoName 排序
  Repository **newGlobalRepos = new Repository *[repoManagement.numRepos + 1];
  int insertIdxGlobal = 0;
  while (insertIdxGlobal < repoManagement.numRepos)
  {
    int ownerCmp = strcmp(repoManagement.repos[insertIdxGlobal]->owner->name, owner->name);
    if (ownerCmp > 0 || (ownerCmp == 0 && strcmp(repoManagement.repos[insertIdxGlobal]->name, repoName) > 0))
    {
      break;
    }
    newGlobalRepos[insertIdxGlobal] = repoManagement.repos[insertIdxGlobal];
    insertIdxGlobal++;
  }

  newGlobalRepos[insertIdxGlobal] = newRepo;
  for (int i = insertIdxGlobal; i < repoManagement.numRepos; ++i)
  {
    newGlobalRepos[i + 1] = repoManagement.repos[i];
  }
  delete[] repoManagement.repos;
  repoManagement.repos = newGlobalRepos;
  repoManagement.numRepos++;

  // cerr << "numrepo is " << repoManagement.numRepos << endl;

  // 4. 维护 User 拥有的仓库数组 (按 repoName 字典序)
  Repository **newUserRepos = new Repository *[owner->numRepos + 1];
  int insertIdxUser = 0;
  while (insertIdxUser < owner->numRepos && strcmp(owner->repos[insertIdxUser]->name, repoName) < 0)
  {
    newUserRepos[insertIdxUser] = owner->repos[insertIdxUser];
    insertIdxUser++;
  }
  newUserRepos[insertIdxUser] = newRepo;
  for (int i = insertIdxUser; i < owner->numRepos; ++i)
  {
    newUserRepos[i + 1] = owner->repos[i];
  }
  delete[] owner->repos;
  owner->repos = newUserRepos;
  owner->numRepos++;

  return insertIdxGlobal;
}

// ─────────────────────────────────────────────────────────────
// Task 3 - Create Branch
// ─────────────────────────────────────────────────────────────
bool create_branch(RepositoryManagement &repoManagement, char *repoFQN,
                   const char *branchName, const User *creator,
                   const Commit *commit)
{
  if (strlen(branchName) > MAX_BRANCH_NAME_LEN)
  {
    cout << "Branch name length exceeded " << MAX_BRANCH_NAME_LEN - 1
         << " characters." << endl;
    return false;
  }

  // Parse repoFQN
  char *buf = new char[strlen(repoFQN) + 1];
  strcpy(buf, repoFQN);
  char *ownerName = strtok(buf, "/");
  char *repoName = strtok(nullptr, "/");

  Repository *repo = nullptr;
  for (int i = 0; i < repoManagement.numRepos; i++)
  {
    Repository *r = repoManagement.repos[i];
    if (strcmp(r->owner->name, ownerName) == 0 &&
        strcmp(r->name, repoName) == 0)
    {
      repo = r;
      break;
    }
  }
  delete[] buf;

  if (repo == nullptr)
  {
    cout << "Repository " << repoFQN << " does not exist." << endl;
    return false;
  }

  // "main" is always implicitly present
  if (strcmp(branchName, "main") == 0)
  {
    cout << "Branch " << repoFQN << ':' << branchName << " already exists." << endl;
    return false;
  }

  // Check for duplicate branch name
  for (int i = 0; i < repo->numBranches; i++)
  {
    if (strcmp(repo->branches[i]->name, branchName) == 0)
    {
      cout << "Branch " << repoFQN << ':' << branchName << " already exists." << endl;
      return false;
    }
  }

  Commit *head = new Commit;

  if (commit == nullptr)
  { // commit is not specified, head is the latest commit of the repo
    *head = *(repo->commits);
    head->next = nullptr;
  }
  else
  { // commit is specified
    *head = *commit;
  }

  Branch *newBranch = new Branch;
  newBranch->creator = creator;
  newBranch->head = head;
  newBranch->repo = repo;
  strcpy(newBranch->name, branchName);

  Branch **newBranches = new Branch *[repo->numBranches + 1];
  int insertIdx = 0;
  while (insertIdx < repo->numBranches && strcmp(repo->branches[insertIdx]->name, branchName) < 0)
  {
    newBranches[insertIdx] = repo->branches[insertIdx];
    insertIdx++;
  }

  newBranches[insertIdx] = newBranch;

  for (int i = insertIdx; i < repo->numBranches; ++i)
  {
    newBranches[i + 1] = repo->branches[i];
  }

  delete[] repo->branches;
  repo->branches = newBranches;
  repo->numBranches++;

  return true;
}

// ─────────────────────────────────────────────────────────────
// Task 4 - Add Commit
// ─────────────────────────────────────────────────────────────
void add_commit(RepositoryManagement &repoManagement, const User *author,
                char *repoFQN, const char *branch, const char *commitMessage,
                time_t timestamp)
{
  if (commitMessage == nullptr || strlen(commitMessage) == 0)
  {
    cout << "Invalid commit message length." << endl;
    return;
  }
  if (author == nullptr || repoFQN == nullptr)
  {
    cout << "Invalid parameters for commit." << endl;
    return;
  }

  // Parse repoFQN
  char *buf = new char[strlen(repoFQN) + 1];
  strcpy(buf, repoFQN);
  char *ownerName = strtok(buf, "/");
  char *repoName = strtok(nullptr, "/");

  Repository *repo = nullptr;
  for (int i = 0; i < repoManagement.numRepos; i++)
  {
    Repository *r = repoManagement.repos[i];
    if (strcmp(r->owner->name, ownerName) == 0 &&
        strcmp(r->name, repoName) == 0)
    {
      repo = r;
      break;
    }
  }
  delete[] buf;

  if (repo == nullptr)
  {
    cout << "Repository " << repoFQN << " does not exist." << endl;
    return;
  }

  // nullptr or "main" both target the main chain
  bool targetingMain = (branch == nullptr || strcmp(branch, "main") == 0);

  Branch *targetBranch = nullptr;
  if (!targetingMain)
  {
    for (int i = 0; i < repo->numBranches; i++)
    {
      if (strcmp(repo->branches[i]->name, branch) == 0)
      {
        targetBranch = repo->branches[i];
        break;
      }
    }
    if (targetBranch == nullptr)
    {
      cout << "Branch " << repoFQN << ':' << branch << " does not exist." << endl;
      return;
    }
  }

  // Determine the previous commit used for the hash, and whether
  // the branch is empty (head == nullptr, meaning no private commits yet).
  // An empty branch uses repo->commits for the hash but must NOT link
  // newCommit->next to it — that link is made by the merge splice later.
  bool branchWasEmpty = false;
  Commit *prevCommit;
  if (targetBranch != nullptr)
  {
    if (targetBranch->head != nullptr)
    {
      prevCommit = targetBranch->head;
    }
    else
    {
      prevCommit = repo->commits; // hash input only
      branchWasEmpty = true;
    }
  }
  else
  {
    prevCommit = repo->commits;
  }

  Commit *newCommit = new Commit;
  newCommit->author = author;
  newCommit->timestamp = timestamp;
  newCommit->next = nullptr;
  newCommit->prev = nullptr;

  if (strlen(commitMessage) > MAX_COMMIT_MSG_LEN)
  {
    cout << "Warning: commit message is longer than " << MAX_COMMIT_MSG_LEN
         << " characters and will be truncated." << endl;
    strncpy(newCommit->message, commitMessage, MAX_COMMIT_MSG_LEN - 1);
    newCommit->message[MAX_COMMIT_MSG_LEN - 1] = '\0';
  }
  else
  {
    strcpy(newCommit->message, commitMessage);
  }

  SHA1 sha;
  reset(sha);
  initialize(sha);
  input(sha, author->name, strlen(author->name));
  input(sha, newCommit->message, strlen(newCommit->message));
  input(sha, timestamp);
  // Mix in previous commit
  input(sha, prevCommit->author->name, strlen(prevCommit->author->name));
  input(sha, prevCommit->message, strlen(prevCommit->message));
  input(sha, prevCommit->timestamp);
  digest(sha);
  newCommit->hash = sha;

  // Prepend to the appropriate chain.
  // For an empty branch, newCommit->next stays nullptr — the merge
  // splice will attach it to repo->commits at merge time.
  if (!branchWasEmpty)
  {
    newCommit->next = prevCommit;
    prevCommit->prev = newCommit;
  }

  if (targetBranch != nullptr)
  {
    targetBranch->head = newCommit;
    cout << "Pushing commit ";
    print_sha(sha);
    cout << " to branch " << repoFQN << ':' << branch << '.' << endl;
  }
  else
  {
    repo->commits = newCommit;
    cout << "Pushing commit ";
    print_sha(sha);
    cout << " to branch " << repoFQN << ":main." << endl;
  }
}

// ─────────────────────────────────────────────────────────────
// Task 5 - Transfer Ownership
// ─────────────────────────────────────────────────────────────
bool transfer_ownership(UserManagement &userManagement,
                        RepositoryManagement &repoManagement,
                        const char *fromUsername, const char *toUsername,
                        const char *repoName)
{
  if (fromUsername == nullptr || toUsername == nullptr || repoName == nullptr)
  {
    cout << "Invalid parameters for ownership transfer." << endl;
    return false;
  }

  User *fromUser = nullptr, *toUser = nullptr;
  for (User *ptr = userManagement.head; ptr != nullptr; ptr = ptr->next)
  {
    if (strcmp(ptr->name, fromUsername) == 0)
      fromUser = ptr;
    if (strcmp(ptr->name, toUsername) == 0)
      toUser = ptr;
  }

  if (fromUser == nullptr)
  {
    cout << "User " << fromUsername << " does not exist." << endl;
    return false;
  }
  if (toUser == nullptr)
  {
    cout << "User " << toUsername << " does not exist." << endl;
    return false;
  }
  if (fromUser == toUser)
  {
    cout << "Why are you transferring ownership to the same user?" << endl;
    return false;
  }

  Repository *targetRepo = nullptr;
  for (int i = 0; i < repoManagement.numRepos; i++)
  {
    Repository *r = repoManagement.repos[i];
    if (r->owner == fromUser && strcmp(r->name, repoName) == 0)
    {
      targetRepo = r;
      break;
    }
  }
  if (targetRepo == nullptr)
  {
    cout << "Repository " << fromUsername << "/" << repoName
         << " not found." << endl;
    return false;
  }

  for (int i = 0; i < toUser->numRepos; i++)
  {
    if (strcmp(toUser->repos[i]->name, repoName) == 0)
    {
      cout << "Repository " << toUsername << "/" << repoName
           << " already exists." << endl;
      return false;
    }
  }

  targetRepo->owner = toUser;

  // Remove from fromUser
  int fc = 0;
  Repository **newFrom = new Repository *[fromUser->numRepos - 1];
  for (int i = 0; i < fromUser->numRepos; i++)
    if (fromUser->repos[i] != targetRepo)
      newFrom[fc++] = fromUser->repos[i];
  delete[] fromUser->repos;
  fromUser->repos = newFrom;
  fromUser->numRepos--;

  // Add to toUser
  Repository **newTo = new Repository *[toUser->numRepos + 1];
  for (int i = 0; i < toUser->numRepos; i++)
    newTo[i] = toUser->repos[i];
  newTo[toUser->numRepos] = targetRepo;
  delete[] toUser->repos;
  toUser->repos = newTo;
  toUser->numRepos++;

  return true;
}

// ─────────────────────────────────────────────────────────────
// Task 6 - Create Pull Request
// pr->toBranch == nullptr means target is main.
// pr->fromBranch == nullptr means source is main.
// ─────────────────────────────────────────────────────────────
bool create_pull_request(const RepositoryManagement &repoManagement,
                         const char *title, const User *author,
                         char *fromBranchFQN, char *toBranchFQN)
{
  if (strlen(title) > MAX_PR_TITLE_LEN)
  {
    cout << "Pull request title length exceeded " << MAX_PR_TITLE_LEN - 1
         << " characters." << endl;
    return false;
  }

  // Parse "owner/repo:branch" for from
  char *fromBuf = new char[strlen(fromBranchFQN) + 1];
  strcpy(fromBuf, fromBranchFQN);
  char *fromRepoFQN = strtok(fromBuf, ":");
  char *fromBranchName = strtok(nullptr, ":");

  // Parse "owner/repo:branch" for to
  char *toBuf = new char[strlen(toBranchFQN) + 1];
  strcpy(toBuf, toBranchFQN);
  char *toRepoFQN = strtok(toBuf, ":");
  char *toBranchName = strtok(nullptr, ":");

  // ── Find fromRepo ──
  char *fromTemp = new char[strlen(fromRepoFQN) + 1];
  strcpy(fromTemp, fromRepoFQN);
  char *fromOwner = strtok(fromTemp, "/");
  char *fromRepoName = strtok(nullptr, "/");

  Repository *fromRepo = nullptr;
  for (int i = 0; i < repoManagement.numRepos; i++)
  {
    Repository *r = repoManagement.repos[i];
    if (strcmp(r->owner->name, fromOwner) == 0 &&
        strcmp(r->name, fromRepoName) == 0)
    {
      fromRepo = r;
      break;
    }
  }
  delete[] fromTemp;

  if (fromRepo == nullptr)
  {
    cout << "Repository " << fromRepoFQN << " does not exist." << endl;
    delete[] fromBuf;
    delete[] toBuf;
    return false;
  }

  // ── Find fromBranch (nullptr = main) ──
  Branch *fromBranch = nullptr;
  if (strcmp(fromBranchName, "main") != 0)
  { // not the main branch
    for (int i = 0; i < fromRepo->numBranches; i++)
    {
      if (strcmp(fromRepo->branches[i]->name, fromBranchName) == 0)
      {
        fromBranch = fromRepo->branches[i];
        break;
      }
    }
    if (fromBranch == nullptr && strcmp(fromBranchName, "main") != 0)
    {
      cout << "Branch " << fromBranchFQN << " does not exist." << endl;
      delete[] fromBuf;
      delete[] toBuf;
      return false;
    }
  }

  // ── Find toRepo ──
  char *toTemp = new char[strlen(toRepoFQN) + 1];
  strcpy(toTemp, toRepoFQN);
  char *toOwner = strtok(toTemp, "/");
  char *toRepoName = strtok(nullptr, "/");

  Repository *toRepo = nullptr;
  for (int i = 0; i < repoManagement.numRepos; i++)
  {
    Repository *r = repoManagement.repos[i];
    if (strcmp(r->owner->name, toOwner) == 0 &&
        strcmp(r->name, toRepoName) == 0)
    {
      toRepo = r;
      break;
    }
  }
  delete[] toTemp;

  if (toRepo == nullptr)
  {
    cout << "Repository " << toRepoFQN << " does not exist." << endl;
    delete[] fromBuf;
    delete[] toBuf;
    return false;
  }

  // ── Find toBranch (nullptr = main) ──
  Branch *toBranch = nullptr;
  if (strcmp(toBranchName, "main") != 0)
  {
    for (int i = 0; i < toRepo->numBranches; i++)
    {
      if (strcmp(toRepo->branches[i]->name, toBranchName) == 0)
      {
        toBranch = toRepo->branches[i];
        break;
      }
    }
    if (toBranch == nullptr && strcmp(toBranchName, "main") != 0)
    {
      cout << "Branch " << toBranchFQN << " does not exist." << endl;
      delete[] fromBuf;
      delete[] toBuf;
      return false;
    }
  }

  // ── Create PR ──
  PullRequest *pr = new PullRequest;
  pr->author = author;
  pr->id = toRepo->numPrs + 1;
  pr->repo = toRepo;
  pr->fromBranch = fromBranch; // nullptr means "main"
  pr->toBranch = toBranch;     // nullptr means "main"
  pr->status = OPEN;
  strncpy(pr->title, title, MAX_PR_TITLE_LEN - 1);
  pr->title[MAX_PR_TITLE_LEN - 1] = '\0';

  PullRequest **newPRs = new PullRequest *[toRepo->numPrs + 1];
  for (int i = 0; i < toRepo->numPrs; i++)
    newPRs[i] = toRepo->prs[i];
  newPRs[toRepo->numPrs] = pr;
  delete[] toRepo->prs;
  toRepo->prs = newPRs;
  toRepo->numPrs++;

  cout << "Pull request #" << pr->id << " has been created in "
       << toRepoFQN << '.' << endl;

  delete[] fromBuf;
  delete[] toBuf;
  return true;
}

// ─────────────────────────────────────────────────────────────
// Task 7 - Fork Repository
// ─────────────────────────────────────────────────────────────
bool fork_repository(UserManagement &userManagement,
                     RepositoryManagement &repoManagement, const User *owner,
                     const char *forkedOwner, const char *repoToFork)
{
  if (owner == nullptr || forkedOwner == nullptr || repoToFork == nullptr)
  {
    cout << "Invalid parameters for fork." << endl;
    return false;
  }

  // Find source repo
  Repository *sourceRepo = nullptr;
  for (int i = 0; i < repoManagement.numRepos; i++)
  {
    Repository *r = repoManagement.repos[i];
    if (r->owner == owner && strcmp(r->name, repoToFork) == 0)
    {
      sourceRepo = r;
      break;
    }
  }
  if (sourceRepo == nullptr)
  {
    cout << "Repository " << owner->name << "/" << repoToFork
         << " does not exist." << endl;
    return false;
  }

  // Find new owner
  User *newOwner = nullptr;
  for (User *ptr = userManagement.head; ptr != nullptr; ptr = ptr->next)
  {
    if (strcmp(ptr->name, forkedOwner) == 0)
    {
      newOwner = ptr;
      break;
    }
  }
  if (newOwner == nullptr)
  {
    cout << "User " << forkedOwner << " does not exist." << endl;
    return false;
  }

  // Check duplicate
  for (int i = 0; i < newOwner->numRepos; i++)
  {
    if (strcmp(newOwner->repos[i]->name, repoToFork) == 0)
    {
      cout << "Repository " << forkedOwner << "/" << repoToFork
           << " already exists." << endl;
      return false;
    }
  }

  // Create forked repo (shares commit chain with source)
  Repository *forkedRepo = new Repository;
  forkedRepo->owner = newOwner;
  forkedRepo->numPrs = 0;
  forkedRepo->numForks = 0;
  forkedRepo->numBranches = 0;
  forkedRepo->prs = nullptr;
  forkedRepo->forks = nullptr;
  forkedRepo->branches = nullptr;
  forkedRepo->commits = sourceRepo->commits; // shared
  strcpy(forkedRepo->name, repoToFork);

  // Copy branches
  if (sourceRepo->numBranches > 0)
  {
    forkedRepo->branches = new Branch *[sourceRepo->numBranches];
    forkedRepo->numBranches = sourceRepo->numBranches;
    for (int i = 0; i < sourceRepo->numBranches; i++)
    {
      Branch *src = sourceRepo->branches[i];
      Branch *nb = new Branch;
      strcpy(nb->name, src->name);
      nb->creator = src->creator;
      nb->head = src->head;
      nb->repo = forkedRepo;
      forkedRepo->branches[i] = nb;
    }
  }

  // Add to global repo array
  Repository **newRepos = new Repository *[repoManagement.numRepos + 1];
  for (int i = 0; i < repoManagement.numRepos; i++)
    newRepos[i] = repoManagement.repos[i];
  newRepos[repoManagement.numRepos] = forkedRepo;
  delete[] repoManagement.repos;
  repoManagement.repos = newRepos;
  repoManagement.numRepos++;

  // Add to newOwner's array
  Repository **newUserRepos = new Repository *[newOwner->numRepos + 1];
  for (int i = 0; i < newOwner->numRepos; i++)
    newUserRepos[i] = newOwner->repos[i];
  newUserRepos[newOwner->numRepos] = forkedRepo;
  delete[] newOwner->repos;
  newOwner->repos = newUserRepos;
  newOwner->numRepos++;

  // Record in source forks list
  Repository **newForks = new Repository *[sourceRepo->numForks + 1];
  for (int i = 0; i < sourceRepo->numForks; i++)
    newForks[i] = sourceRepo->forks[i];
  newForks[sourceRepo->numForks] = forkedRepo;
  delete[] sourceRepo->forks;
  sourceRepo->forks = newForks;
  sourceRepo->numForks++;

  cout << "Fork " << forkedOwner << "/" << repoToFork
       << " created successfully." << endl;
  return true;
}

// ─────────────────────────────────────────────────────────────
// Helper: remove a branch from its repo's branches[] and free it
// ─────────────────────────────────────────────────────────────
static void remove_branch(Branch *br)
{
  if (br == nullptr)
    return;
  Repository *repo = br->repo;
  int idx = -1;
  for (int i = 0; i < repo->numBranches; i++)
  {
    if (repo->branches[i] == br)
    {
      idx = i;
      break;
    }
  }
  if (idx == -1)
    return;

  int newLen = repo->numBranches - 1;
  Branch **nb = newLen > 0 ? new Branch *[newLen] : nullptr;
  for (int i = 0, j = 0; i < repo->numBranches; i++)
    if (i != idx)
      nb[j++] = repo->branches[i];
  delete br;
  delete[] repo->branches;
  repo->branches = nb;
  repo->numBranches = newLen;
}

// ─────────────────────────────────────────────────────────────
// Task 8.1 - Squash Merge
// Squashes all source-branch commits into one new commit whose
// message is "<PR title> (#<id>)", then deletes the source branch.
// ─────────────────────────────────────────────────────────────
void merge_pull_request_squashmerge(RepositoryManagement &repoManagement,
                                    char *repoFQN, int prNumber,
                                    time_t timestamp)
{
  // Find repo
  Repository *repo = nullptr;
  char *buf = new char[strlen(repoFQN) + 1];
  strcpy(buf, repoFQN);
  char *ownerName = strtok(buf, "/");
  char *repoName = strtok(nullptr, "/");
  for (int i = 0; i < repoManagement.numRepos; i++)
  {
    Repository *r = repoManagement.repos[i];
    if (strcmp(r->owner->name, ownerName) == 0 &&
        strcmp(r->name, repoName) == 0)
    {
      repo = r;
      break;
    }
  }
  delete[] buf;

  if (repo == nullptr)
  {
    cout << "Repository " << repoFQN << " does not exist." << endl;
    return;
  }
  if (prNumber <= 0 || prNumber > repo->numPrs)
  {
    cout << "Invalid pull request number for repository " << repoFQN
         << ": out of range." << endl;
    return;
  }

  PullRequest *pr = repo->prs[prNumber - 1];
  if (pr->status != OPEN)
  {
    cout << "Pull request is not open." << endl;
    return;
  }

  // Build squash commit message: "<title> (#<id>)"
  char msg[MAX_COMMIT_MSG_LEN];
  snprintf(msg, MAX_COMMIT_MSG_LEN, "%s (#%d)", pr->title, pr->id);

  // Previous commit on the target (main or named branch)
  Commit *prevCommit = (pr->toBranch != nullptr) ? pr->toBranch->head
                                                 : repo->commits;

  Commit *squash = new Commit;
  squash->author = pr->author;
  squash->timestamp = timestamp;
  squash->next = nullptr;
  squash->prev = nullptr;
  strcpy(squash->message, msg);

  SHA1 sha;
  reset(sha);
  initialize(sha);
  input(sha, pr->author->name, strlen(pr->author->name));
  input(sha, squash->message, strlen(squash->message));
  input(sha, timestamp);
  // Mix in previous commit
  input(sha, prevCommit->author->name, strlen(prevCommit->author->name));
  input(sha, prevCommit->message, strlen(prevCommit->message));
  input(sha, prevCommit->timestamp);
  digest(sha);
  squash->hash = sha;

  // Prepend to main chain
  squash->next = repo->commits;
  if (repo->commits)
    repo->commits->prev = squash;
  repo->commits = squash;

  // Update named target branch head if applicable
  if (pr->toBranch != nullptr)
    pr->toBranch->head = squash;

  // Delete source branch
  remove_branch(pr->fromBranch);
  pr->fromBranch = nullptr;

  pr->status = MERGED;
  cout << "Pull request #" << prNumber << " in " << repoFQN
       << " has been merged using squash merge." << endl;
}

// ─────────────────────────────────────────────────────────────
// Task 8.2 - Rebase Merge
// Splices the source-branch commit chain verbatim in front of
// repo->commits, then deletes the source branch.
// ─────────────────────────────────────────────────────────────
void merge_pull_request_rebasemerge(RepositoryManagement &repoManagement,
                                    char *repoFQN, int prNumber,
                                    time_t timestamp)
{
  // Find repo
  Repository *repo = nullptr;
  char *buf = new char[strlen(repoFQN) + 1];
  strcpy(buf, repoFQN);
  char *ownerName = strtok(buf, "/");
  char *repoName = strtok(nullptr, "/");
  for (int i = 0; i < repoManagement.numRepos; i++)
  {
    Repository *r = repoManagement.repos[i];
    if (strcmp(r->owner->name, ownerName) == 0 &&
        strcmp(r->name, repoName) == 0)
    {
      repo = r;
      break;
    }
  }
  delete[] buf;

  if (repo == nullptr)
  {
    cout << "Repository " << repoFQN << " does not exist." << endl;
    return;
  }
  if (prNumber <= 0 || prNumber > repo->numPrs)
  {
    cout << "Invalid pull request number for repository " << repoFQN
         << ": out of range." << endl;
    return;
  }

  PullRequest *pr = repo->prs[prNumber - 1];
  if (pr->status != OPEN)
  {
    cout << "Pull request is not open." << endl;
    return;
  }
  if (pr->fromBranch == nullptr)
  {
    cout << "Invalid branch in pull request." << endl;
    return;
  }

  // Walk to the tail of the branch's commit chain.
  // create_branch copies the main head into a new node at the tail —
  // that node (branchTail) is a duplicate of the base commit and must
  // be excluded from the splice and then freed.
  Commit *branchHead = pr->fromBranch->head;
  Commit *branchTail = branchHead;
  while (branchTail->next != nullptr)
    branchTail = branchTail->next;

  // secondLastCommit is the oldest *real* branch commit.
  // For a single-commit branch: secondLastCommit == branchHead.
  Commit *secondLastCommit = branchTail->prev;

  Commit *oldRepoHead = repo->commits;

  if (pr->toBranch == nullptr)
  {
    // Detach the duplicate base-commit copy (branchTail) and free it.
    secondLastCommit->next = nullptr;
    delete branchTail;
    branchTail = nullptr;

    // Splice real branch commits in front of repo->commits.
    secondLastCommit->next = oldRepoHead;
    if (oldRepoHead)
      oldRepoHead->prev = secondLastCommit;
    repo->commits = branchHead;
  }
  else
  {
    pr->toBranch->head = branchHead;
  }

  // Delete source branch (commits now owned by main chain).
  remove_branch(pr->fromBranch);
  pr->fromBranch = nullptr;

  // Recompute hashes for each rebased commit.
  // The chain is newest-first: head->next is the older commit.
  // Each commit's hash is derived from its own fields plus its older
  // neighbour's fields — i.e., newCommit->next, NOT newCommit->prev.
  for (Commit *cur = repo->commits; cur != oldRepoHead; cur = cur->next)
  {
    cur->timestamp = timestamp;
    Commit *parent = cur->next; // older commit (parent in git terms)
    SHA1 sha;
    reset(sha);
    initialize(sha);
    input(sha, cur->author->name, strlen(cur->author->name));
    input(sha, cur->message, strlen(cur->message));
    input(sha, cur->timestamp);
    input(sha, parent->author->name, strlen(parent->author->name));
    input(sha, parent->message, strlen(parent->message));
    input(sha, parent->timestamp);
    digest(sha);
    cur->hash = sha;
  }

  pr->status = MERGED;
  cout << "Pull request #" << prNumber << " in " << repoFQN
       << " has been merged using rebase merge." << endl;
}
// ─────────────────────────────────────────────────────────────
// Task 8.3 - Merge Commit
// Splices source-branch commits in front of repo->commits, then
// prepends a new merge commit on top. Deletes the source branch.
// ─────────────────────────────────────────────────────────────
void merge_pull_request_mergecommit(RepositoryManagement &repoManagement,
                                    char *repoFQN, int prNumber,
                                    time_t timestamp)
{
  Repository *repo = nullptr;
  char *buf = new char[strlen(repoFQN) + 1];
  strcpy(buf, repoFQN);
  char *ownerName = strtok(buf, "/");
  char *repoName = strtok(nullptr, "/");
  for (int i = 0; i < repoManagement.numRepos; i++)
  {
    Repository *r = repoManagement.repos[i];
    if (strcmp(r->owner->name, ownerName) == 0 &&
        strcmp(r->name, repoName) == 0)
    {
      repo = r;
      break;
    }
  }
  delete[] buf;
 
  if (repo == nullptr)
  {
    cout << "Repository " << repoFQN << " does not exist." << endl;
    return;
  }
  if (prNumber <= 0 || prNumber > repo->numPrs)
  {
    cout << "Invalid pull request number for repository " << repoFQN
         << ": out of range." << endl;
    return;
  }
 
  PullRequest *pr = repo->prs[prNumber - 1];
 
  // FIX Bug 1: guard against already-closed PR and null source branch
  if (pr->status != OPEN)
  {
    cout << "Pull request is not open." << endl;
    return;
  }
  if (pr->fromBranch == nullptr)
  {
    cout << "Invalid branch in pull request." << endl;
    return;
  }
 
  // FIX Bug 4: capture message fields BEFORE touching the branch
  char msg[MAX_COMMIT_MSG_LEN];
  snprintf(msg, MAX_COMMIT_MSG_LEN,
           "Merge pull request #%d from branch %s/%s:%s",
           pr->id,
           pr->fromBranch->repo->owner->name,
           pr->fromBranch->repo->name,
           pr->fromBranch->name);
 
  // 1. Splice source branch commits in front of repo->commits
  //    create_branch copies the main head into a new node at branchTail —
  //    that duplicate must be detached and freed before splicing.
  Commit *branchHead = pr->fromBranch->head;
  Commit *branchTail = branchHead;
  while (branchTail->next != nullptr)
    branchTail = branchTail->next;
 
  // FIX Bug 2: detach and free the duplicate base-commit copy (branchTail)
  Commit *secondLastCommit = branchTail->prev;
  secondLastCommit->next = nullptr;
  delete branchTail;
  branchTail = nullptr;
 
  Commit *oldRepoHead = repo->commits;
 
  // Splice: oldest real branch commit -> old repo head
  secondLastCommit->next = oldRepoHead;
  if (oldRepoHead)               // FIX Bug 2: guard against null repo->commits
    oldRepoHead->prev = secondLastCommit;
 
  // 2. Create the merge commit on top
  Commit *mergeCommit = new Commit;
  mergeCommit->author    = pr->author;
  mergeCommit->timestamp = timestamp;
  mergeCommit->prev      = nullptr;
  mergeCommit->next      = branchHead;
  branchHead->prev       = mergeCommit;
  strcpy(mergeCommit->message, msg);
 
  // FIX Bug 3: hash mergeCommit (not branchHead) using branchHead as parent
  SHA1 sha;
  reset(sha);
  initialize(sha);
  input(sha, mergeCommit->author->name, strlen(mergeCommit->author->name));
  input(sha, mergeCommit->message,      strlen(mergeCommit->message));
  input(sha, mergeCommit->timestamp);
  input(sha, branchHead->author->name,  strlen(branchHead->author->name));
  input(sha, branchHead->message,       strlen(branchHead->message));
  input(sha, branchHead->timestamp);
  digest(sha);
  mergeCommit->hash = sha;
 
  repo->commits = mergeCommit;
 
  // Update named target branch head if applicable
  if (pr->toBranch != nullptr)
    pr->toBranch->head = mergeCommit;
 
  // Delete source branch (commits now owned by main chain)
  remove_branch(pr->fromBranch);
  pr->fromBranch = nullptr;
 
  pr->status = MERGED;
  cout << "Pull request #" << prNumber << " in " << repoFQN
       << " has been merged using a merge commit." << endl;
}


// ─────────────────────────────────────────────────────────────
// Task 9 - De-register User
// ─────────────────────────────────────────────────────────────
void deregister_user(UserManagement &userManagement,
                     RepositoryManagement &repoManagement,
                     const char *username)
{
  // Step 1: Find target user
  User *target = nullptr;
  User *prev = nullptr;
  for (User *ptr = userManagement.head; ptr != nullptr; ptr = ptr->next)
  {
    if (strcmp(ptr->name, username) == 0)
    {
      target = ptr;
      break;
    }
    prev = ptr;
  }
  if (target == nullptr)
  {
    cout << "User " << username << " does not exist." << endl;
    return;
  }

  // Step 2: Ghost-replace every reference to target across all repos
  for (int i = 0; i < repoManagement.numRepos; i++)
  {
    Repository *repo = repoManagement.repos[i];
    for (Commit *c = repo->commits; c; c = c->next)
      if (c->author == target)
        c->author = ghost;
    for (int b = 0; b < repo->numBranches; b++)
      for (Commit *c = repo->branches[b]->head; c; c = c->next)
        if (c->author == target)
          c->author = ghost;
    for (int p = 0; p < repo->numPrs; p++)
      if (repo->prs[p]->author == target)
        repo->prs[p]->author = ghost;
    for (int b = 0; b < repo->numBranches; b++)
      if (repo->branches[b]->creator == target)
        repo->branches[b]->creator = ghost;
  }

  // Step 3: Delete every repo owned by target
  for (int i = 0; i < repoManagement.numRepos; i++)
  {
    Repository *repo = repoManagement.repos[i];
    if (repo->owner != target)
      continue;

    // 3a. Remove from every other repo's forks[] to avoid dangling pointers
    for (int j = 0; j < repoManagement.numRepos; j++)
    {
      Repository *other = repoManagement.repos[j];
      if (other == repo || other == nullptr)
        continue;
      int fIdx = -1;
      for (int f = 0; f < other->numForks; f++)
        if (other->forks[f] == repo)
        {
          fIdx = f;
          break;
        }
      if (fIdx == -1)
        continue;
      int newForkCount = other->numForks - 1;
      Repository **newForks = newForkCount > 0 ? new Repository *[newForkCount] : nullptr;
      for (int f = 0, g = 0; f < other->numForks; f++)
        if (f != fIdx)
          newForks[g++] = other->forks[f];
      delete[] other->forks;
      other->forks = newForks;
      other->numForks = newForkCount;
    }

    // 3b. Free branches (directly — not via remove_branch which modifies the array mid-loop)
    //     Skip commit nodes if forks share them
    for (int b = 0; b < repo->numBranches; b++)
    {
      Branch *br = repo->branches[b];
      if (repo->numForks == 0)
      {
        for (Commit *cur = br->head; cur;)
        {
          Commit *nxt = cur->next;
          delete cur;
          cur = nxt;
        }
      }
      delete br;
    }
    delete[] repo->branches;

    // 3c. Free PRs
    for (int p = 0; p < repo->numPrs; p++)
      delete repo->prs[p];
    delete[] repo->prs;

    // 3d. Free forks pointer array (fork repos themselves survive)
    delete[] repo->forks;

    // 3e. Free main commit chain only if no forks share it
    if (repo->numForks == 0)
    {
      for (Commit *cur = repo->commits; cur;)
      {
        Commit *nxt = cur->next;
        delete cur;
        cur = nxt;
      }
    }

    delete repo;
    repoManagement.repos[i] = nullptr;
  }

  // Step 4: Compact repoManagement.repos
  int newCount = 0;
  for (int i = 0; i < repoManagement.numRepos; i++)
    if (repoManagement.repos[i] != nullptr)
      newCount++;
  Repository **newRepos = newCount > 0 ? new Repository *[newCount] : nullptr;
  for (int i = 0, j = 0; i < repoManagement.numRepos; i++)
    if (repoManagement.repos[i] != nullptr)
      newRepos[j++] = repoManagement.repos[i];
  delete[] repoManagement.repos;
  repoManagement.repos = newRepos;
  repoManagement.numRepos = newCount;

  // Step 5: Splice user out and free
  if (prev == nullptr)
    userManagement.head = target->next;
  else
    prev->next = target->next;
  delete[] target->repos;
  delete target;

  cout << "User " << username << " has been deregistered." << endl;
}