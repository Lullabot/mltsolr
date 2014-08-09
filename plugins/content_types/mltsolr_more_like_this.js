/**
 * AngularJS module for More Like This.
 */
if (typeof angular !== 'undefined') {
  angular.module('mltsearch.morelikethis', ['ngSanitize'])
  /**
   * Controller that requests a list of related teasers and hands it to the view.
   */
  .controller('moreLikeThisController', ['$scope', '$element', '$http', function ($scope, $element, $http) {
    /**
     * Requests related content for a given node.
     *
     * @param int id
     *   nid of the node.
     */
    $scope.loadRelatedContent = function(id) {
      $http.get('/api/1.0/more_like_this/' + id, {
        cache : true
      })
      .success(function(data, status, headers, config) {
        $scope.teasers = data;
      }).
      error(function(eventData, status, headers, config) {
        // Silent. No need to inform the user.
        $scope.teasers = null;
      });
    };

    // Initial load of related content.
    $scope.contentId = $element.data('content-id');
    $scope.loadRelatedContent($scope.contentId);

    // Load related content if the content id changes which means that a new
    // node is being viewed.
    $scope.$watch('contentId', function(newValue, oldValue) {
      $scope.loadRelatedContent(newValue);
    });
  }]);
}
