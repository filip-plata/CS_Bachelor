from rest_framework import serializers
from taggit_serializer.serializers import TagListSerializerField, TaggitSerializer

from bargainflow.models import Commission, CommissionBid


class CommissionSerializer(TaggitSerializer, serializers.ModelSerializer):
    tags = TagListSerializerField()
    orderer = serializers.PrimaryKeyRelatedField(
        read_only=True,
        default=serializers.CurrentUserDefault()
    )

    class Meta:
        model = Commission
        exclude = ('date_added', 'orderer')


class CommissionBidSerializer(serializers.ModelSerializer):
    bidder = serializers.PrimaryKeyRelatedField(
        read_only=True,
        default=serializers.CurrentUserDefault()
    )

    class Meta:
        model = CommissionBid
        exclude = ('date_added',)
