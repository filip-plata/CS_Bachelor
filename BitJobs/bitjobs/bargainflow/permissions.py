from rest_framework import permissions

from bargainflow.models import Commission


class CommissionPermission(permissions.BasePermission):

    def has_permission(self, request, view):
        result = True
        unsafe_methods = ['POST', 'PUT']

        if request.method in unsafe_methods:
            result &= request.user.is_authenticated()

        return result

    def has_object_permission(self, request, view, obj):
        result = True

        if request.method in permissions.SAFE_METHODS:
            return True

        result &= request.user == obj.orderer

        if not request.data.get('status') is None:
            trans = request.data.get('status')
            result &= trans in Commission.VALID_TRANSITION[obj.status]

        return result
