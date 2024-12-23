
# Contributing Guide

Thank you for your interest in contributing to our project! To ensure a smooth collaboration process, we've outlined guidelines for contributing, including how to create issues, branch from these issues, and merge your changes.

## Creating an Issue

When reporting a new issue or suggesting a feature, please follow these steps:

1. **Issue Title Nomenclature**:
    - Format: `[Sprint {no.}] - [Component Acronym] [Title of Item Being Worked On]`
    - Example: `[Sprint 4] - [UI] Implement Login Screen`

    ![Issue Title Example](https://github.com/nus-cs3203/23s2-cp-spa-team-01/assets/77258736/5ff5090b-4182-4495-80a1-44c81eea4067)


2. **Select Tags**: Choose relevant tags to categorize your issue, such as `bug`, `feature request`, or `documentation`.

3. **Select Milestone Label**: Assign the issue to a milestone that matches the sprint or phase of the project it belongs to.

4. **Select Our Project**: Link your issue to the appropriate GitHub Project for visibility and tracking.

5. **Submit the Issue**: Review your entries and click on "Submit new issue".

## Creating a Branch from Your Issue

To ensure consistency and traceability, follow these steps to create a branch for your work:

1. **Navigate to Your Created Issue**: Find the issue you created in the Issues tab.

2. **Under 'Development'**: On the right-hand side, click on "Create a branch".

3. **Branch Naming Convention**:
    - The branch name will automatically follow the format: `{issue no.}-Sprint-{no.}-{Component Acronym}-{Title}`
    - Ensure it adheres to this nomenclature for consistency.

    ![Branch Naming Example](https://github.com/nus-cs3203/23s2-cp-spa-team-01/assets/77258736/9ea9dd7b-c717-4141-aa63-e80540deee43)

r_branch_naming.png "Branch naming convention")

4. **Change Branch Source**:
    - Click "Change branch source" and select the appropriate base branch, typically `release/sprint_{no.}`.

5. **Completion**: You've successfully created a branch for your issue.


## When Merging

To integrate your changes back into the project:

1. **Create a Pull Request (PR)**:
    - Target your PR to the `origin release/sprint-{no.}` branch of the project.
    - Ensure your PR description clearly describes the changes and links back to the original issue for traceability.

2. **Issue Closure**:
    - The related issue should be closed automatically once the PR is merged. If not, manually close the issue, referencing the merged PR.


## Merging Consolidated Sprint Branches into the Main Branch

Since our master branch is protected to ensure stability and integrity, all changes must be merged through pull requests (PRs) from designated release branches. This section outlines the process for merging consolidated sprint work into the main branch.

### Preparing for Merge

1. **Consolidation**: Ensure all work for the current sprint has been merged into the release branch (e.g., `release/sprint-{no.}`). This includes completing all PR reviews and ensuring CI checks pass.

2. **Final Testing**: Conduct thorough testing on the release branch to ensure all new features, fixes, and changes function as expected and do not introduce regressions.

3. **Documentation Update**: Verify that all necessary documentation has been updated to reflect any changes or new features being merged into the main branch.

### Creating a Pull Request

1. **Initiate a Pull Request**:
    - Navigate to the `Pull Requests` tab in the GitHub repository.
    - Click on `New pull request`.
    - Set the `base` branch to `master` and the `compare` branch to your current release branch (e.g., `release/sprint-{no.}`).


2. **PR Description**:
    - Clearly describe the scope of changes, features, or fixes being merged. Include references to any related issues or sprint goals.

3. **Review and Approvals**:
    - Request reviews from team members, ensuring at least one approval from project maintainers or designated reviewers. Address any feedback or required changes promptly.

### Merging into Master

1. **Final Checks**:
    - Ensure all required checks, tests, and reviews have been passed and obtained before proceeding with the merge.

2. **Merge Pull Request**:
    - Once approved and all checks pass, the designated project maintainer or a team member with the necessary permissions can merge the pull request into the master branch.


3. **Post-Merge Tasks**:
    - After merging, monitor the application or system for any unexpected issues resulting from the new changes.
    - Update any project tracking tools or sprint boards to reflect the completion of the sprint's work.

### Conclusion

Merging consolidated sprint work into the main branch is a critical step in our development process. By adhering to these guidelines, we ensure that our master branch remains stable and that new changes are integrated smoothly and efficiently.


## Conclusion

By following these guidelines, you help maintain the project's organization and ensure that your contributions can be efficiently reviewed and integrated. Thank you for contributing!
